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
    FileChooser chooser("Load Settings", File::nonexistent, "*.gene");
    if (chooser.browseForFileToOpen()) {
        mainElement = XmlDocument::parse(File(chooser.getResult().getFullPathName() + "/Settings.xml"));
        workingDir = chooser.getResult();
        save = File(chooser.getResult().getFullPathName() + "/Settings.xml");
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
    FileChooser chooser("Save Settings", File::nonexistent, "*.gene");
    if (!chooser.browseForFileToSave(true))
        return;
    File path = chooser.getResult();
    if (path == workingDir) {
        SaveSettings();
        return;
    }
    if (path.exists())
        path.deleteRecursively();
    path.createDirectory();

    UpdateFromUI();
    mainElement->writeToFile(File(path.getFullPathName() + "/Settings.xml"), String::empty);
    File audiodir(path.getFullPathName() + "/Audio");
    audiodir.createDirectory();
    File(path.getFullPathName() + "/Data").createDirectory();

    File(workingDir.getFullPathName() + "/Audio").copyDirectoryTo(audiodir);

    save = File(path.getFullPathName() + "/Settings.xml");
    workingDir = path;
}

void Settings::CreateNew()
{
    FileChooser chooser("Create Settings", File::nonexistent, "*.gene");
    if (!chooser.browseForFileToSave(true))
        return;
    File path = chooser.getResult();
    if (path.exists())
        path.deleteRecursively();
    path.createDirectory();

    mainElement = XmlDocument::parse(BinaryData::GenericSave_xml);
    mainElement->writeToFile(File(path.getFullPathName() + "/Settings.xml"), String::empty);
    File(path.getFullPathName() + "/Audio").createDirectory();
    File(path.getFullPathName() + "/Data").createDirectory();
    workingDir = path;
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

bool Settings::IsLoaded() const { return isLoaded; }

void Settings::LoadAudio()
{
    FileChooser chooser("Load An Audio File", File::nonexistent, "*.aif");
    if (!chooser.browseForFileToOpen())
        return;
    File path = chooser.getResult();

    File destination(workingDir.getFullPathName() + "/Audio/" + path.getFileNameWithoutExtension() + ".aif");
    if (!destination.exists()) {
        path.copyFileTo(destination);
    }
    listeners.call(&Listener::SettingsChanged, this);
}

void Settings::DeleteAudio(String Name) const
{
    File path = workingDir.getFullPathName() + "/Audio/" + Name + ".aif";
    if (path.existsAsFile())
        path.deleteFile();
}

StringArray Settings::GetAudioBin() const
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

std::vector<double> Settings::GetGraph(String Name, int Size) const
{
    std::vector<FFT::Complex> graph;
    XmlElement* data = mainElement->getChildByName(Name);
    forEachXmlChildElement(*data, d)
    {
        graph.emplace_back(FFT::Complex{ (float)d->getDoubleAttribute("value"), (float)d->getDoubleAttribute("loc") });
    }
    std::vector<double> out;
    for (int i = 0; i < Size; i++) {
        out.emplace_back(Interpolate((double)i / (double)Size, graph));
    }
    return out;
}

std::vector<FFTW::AudioAnalysis> Settings::GetAudioData(String Name, int FFTSize, int NumBands, int Band) const
{
    std::vector<File> files;
    XmlElement* data = mainElement->getChildByName(Name);
    forEachXmlChildElement(*data, d)
    {
        files.emplace_back(File(workingDir.getFullPathName() + "/Audio/" + d->getStringAttribute("value") + ".aif"));
    }
    FFTW::AudioLoader loader(FFTSize);
    std::vector<FFTW::AudioAnalysis> out;
    for (int f = 0; f < files.size(); f++) {
        out.emplace_back(loader.AnalyzeAudio(files[f], NumBands, Band));
    }
    return out;
}

std::vector<int> Settings::GetAudioGraph(String Name, int Size) const
{
    std::vector<int> out;
    std::vector<double> positions;

    XmlElement* data = mainElement->getChildByName(Name);
    forEachXmlChildElement(*data, d)
    {
        positions.emplace_back(d->getDoubleAttribute("loc"));
    }

    int pos = 0;
    for (int i = 0; i < Size; i++) {
        if (pos < positions.size()) {
            if ((double)i / (double)Size >= positions[pos + 1])
                pos++;
        }
        out.emplace_back(pos);
    }
    return out;
}

std::vector<String> Settings::GetAudioNameGraph(String Name) const
{
    std::vector<String> out;
    XmlElement* data = mainElement->getChildByName(Name);
    forEachXmlChildElement(*data, d)
    {
        out.emplace_back(d->getStringAttribute("value").trimCharactersAtEnd(".aif"));
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

double Settings::Interpolate(double Position, std::vector<FFT::Complex> Data) const
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