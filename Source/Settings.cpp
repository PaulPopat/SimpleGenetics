/*
  ==============================================================================

    Settings.cpp
    Created: 5 Aug 2015 12:07:13pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "Settings.h"

double LinearInterp(double Distance, double Value1, double Value2)
{
    double difference = Value2 - Value1;
    return Value1 + (difference * Distance);
}

Settings::Settings()
{
    Reset();
}

void Settings::LoadSettings()
{
    FileChooser chooser("Load Settings", File::nonexistent, "*.xml");
    if (chooser.browseForFileToOpen()) {
        mainElement = XmlDocument::parse(chooser.getResult());
        workingDir = chooser.getResult().getParentDirectory();
        save = chooser.getResult();
        isLoaded = true;
        listeners.call(&Listener::SettingsChanged, this);
    }
    else
        Reset();
}

void Settings::SaveSettings()
{
    if (!isLoaded)
        return;

    UpdateFromUI();
    mainElement->writeToFile(save, String::empty);
}

void Settings::SaveSettingsAs()
{
    if (!isLoaded)
        return;
    FileChooser chooser("Save Settings", File::nonexistent, "*.xml");
    if (!chooser.browseForFileToSave(true))
        return;
    save = chooser.getResult();

    UpdateFromUI();
    mainElement->writeToFile(save, String::empty);
    File(save.getParentDirectory().getFullPathName() + "/Audio").createDirectory();
    File(save.getParentDirectory().getFullPathName() + "/Data").createDirectory();
    File(save.getParentDirectory().getFullPathName() + "/Output").createDirectory();

    DirectoryIterator iter(File(workingDir.getFullPathName() + "/Audio"), true, "*.aif");
    while (iter.next()) {
        File res(iter.getFile());
        String destination = save.getParentDirectory().getFullPathName() + "/Audio/" + res.getFileName();
        if (!File(destination).exists()) {
            std::ifstream src(res.getFullPathName().toRawUTF8(), std::ios::binary);
            std::ofstream dst(destination.toRawUTF8(), std::ios::binary);
            dst << src.rdbuf();
        }
    }
    workingDir = save.getParentDirectory();
}

void Settings::CreateNew()
{
    FileChooser chooser("Create Settings", File::nonexistent, "*.xml");
    if (!chooser.browseForFileToSave(true))
        return;
    File path = chooser.getResult();

    mainElement = XmlDocument::parse(BinaryData::GenericSave_xml);
    mainElement->writeToFile(path, String::empty);
    File(path.getParentDirectory().getFullPathName() + "/Audio").createDirectory();
    File(path.getParentDirectory().getFullPathName() + "/Data").createDirectory();
    File(path.getParentDirectory().getFullPathName() + "/Output").createDirectory();
    workingDir = path.getParentDirectory();
    save = path;
    isLoaded = true;
    listeners.call(&Listener::SettingsChanged, this);
}

void Settings::Reset()
{
    mainElement = new XmlElement("Gene");
    workingDir = File::getCurrentWorkingDirectory();
    isLoaded = false;
    listeners.call(&Listener::SettingsChanged, this);
}

bool Settings::IsLoaded() { return isLoaded; }

void Settings::LoadAudio()
{
    FileChooser chooser("Load An Audio File", File::nonexistent, "*.aif");
    if (!chooser.browseForFileToOpen())
        return;
    File path = chooser.getResult();

    String destination = workingDir.getFullPathName() + "/Audio/" + path.getFileNameWithoutExtension() + ".aif";
    if (!File(destination).exists()) {
        std::ifstream src(path.getFullPathName().toRawUTF8(), std::ios::binary);
        std::ofstream dst(destination.toRawUTF8(), std::ios::binary);
        dst << src.rdbuf();
    }
    listeners.call(&Listener::SettingsChanged, this);
}

void Settings::DeleteAudio(String Name)
{
    File path = workingDir.getFullPathName() + "/Audio/" + Name + ".aif";
    if (path.existsAsFile())
        path.deleteFile();
}

StringArray Settings::GetAudioBin()
{
    StringArray output;
    DirectoryIterator iter(File(workingDir.getFullPathName() + "/Audio"), true, "*.aif");
    while (iter.next()) {
        File res(iter.getFile());
        output.add(res.getFileNameWithoutExtension());
    }
    return output;
}

double Settings::GetDoubleValue(String Name) const
{
    return mainElement->getChildByName(Name)->getDoubleAttribute("value");
}

int Settings::GetIntValue(String Name) const
{
    return mainElement->getChildByName(Name)->getIntAttribute("value");
}

String Settings::GetStringValue(String Name) const
{
    return mainElement->getChildByName(Name)->getStringAttribute("value");
}

Array<double> Settings::GetGraph(String Name, int Size) const
{
    Array<FFT::Complex> graph;
    XmlElement* data = mainElement->getChildByName(Name);
    forEachXmlChildElement(*data, d)
    {
        graph.add(FFT::Complex{ (float)d->getDoubleAttribute("value"), (float)d->getDoubleAttribute("loc") });
    }
    Array<double> out;
    for (int i = 0; i < Size; i++) {
        out.add(Interpolate((double)i / (double)Size, graph));
    }
    return out;
}

Array<FFTW::AudioAnalysis> Settings::GetAudioData(String Name, int FFTSize, int NumBands, int Band) const
{
    Array<File> files;
    XmlElement* data = mainElement->getChildByName(Name);
    forEachXmlChildElement(*data, d)
    {
        files.add(File(workingDir.getFullPathName() + "/Audio/" + d->getStringAttribute("value") + ".aif"));
    }
    FFTW::AudioLoader loader(FFTSize);
    Array<FFTW::AudioAnalysis> out;
    for (int f = 0; f < files.size(); f++) {
        out.add(loader.AnalyzeAudio(files[f], NumBands, Band));
    }
    return out;
}

Array<int> Settings::GetAudioGraph(String Name, int Size) const
{
    Array<int> out;
    Array<double> positions;

    XmlElement* data = mainElement->getChildByName(Name);
    forEachXmlChildElement(*data, d)
    {
        positions.add(d->getDoubleAttribute("loc"));
    }

    int pos = 0;
    for (int i = 0; i < Size; i++) {
        if (pos < positions.size()) {
            if ((double)i / (double)Size >= positions[pos + 1])
                pos++;
        }
        out.add(pos);
    }
    return out;
}

Array<String> Settings::GetAudioNameGraph(String Name) const
{
    Array<String> out;
    XmlElement* data = mainElement->getChildByName(Name);
    forEachXmlChildElement(*data, d)
    {
        out.add(d->getStringAttribute("value").trimCharactersAtEnd(".aif"));
    }
    return out;
}

XmlElement Settings::GetRawData(String Name) const
{
    return *mainElement->getChildByName(Name);
}

void Settings::AddXmlElement(XmlElement* ToAdd)
{
    if (!mainElement->replaceChildElement(mainElement->getChildByName(ToAdd->getTagName()), ToAdd)) {
        mainElement->addChildElement(ToAdd);
    }
}

const File& Settings::GetWorkingDirectory() const
{
    return workingDir;
}

void Settings::AddListener(Listener* l)
{
    listeners.add(l);
    listeners.call(&Listener::SettingsChanged, this);
}

void Settings::UpdateFromUI()
{
    listeners.call(&Listener::GetSettings, this);
}

bool Settings::WarningAccepted(String warning)
{
    AlertWindow alert("Warning!", warning, AlertWindow::AlertIconType::QuestionIcon);
    alert.addButton("Ok", true, KeyPress(), KeyPress());
    alert.addButton("Cancel", false, KeyPress(), KeyPress());
    alert.addToDesktop();
    alert.setBounds(230, 150, 650, 150);
    bool toContinue = alert.runModalLoop();
    return toContinue;
}

double Settings::Interpolate(double Position, Array<FFT::Complex> Data) const
{
    double output = Data[Data.size() - 1].r;
    int i = 0;
    while (i < Data.size()) {
        if (i == 0 && Position < Data[i].i)
            return Data[i].r;
        else if (Position < Data[i].i) {
            double distance = (Position - Data[i - 1].i) / (Data[i].i - Data[i - 1].i);
            return LinearInterp(distance, Data[i - 1].r, Data[i].r);
        }
        else if (Position == Data[i].i) {
            return Data[i].r;
        }
        i++;
    }
    return output;
}