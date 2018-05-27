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
    if (chooser.browseForFileToOpen())
    {
        mainElement = XmlDocument::parse(File(chooser.getResult().getFullPathName()));
        workingDir = chooser.getResult().getParentDirectory();
        save = File(chooser.getResult().getFullPathName());
        isLoaded = true;
        listeners.call(&Listener::SettingsChanged, this);
    }
    else
        Reset();
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
    if (!destination.exists())
    {
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
    while (iter.next())
    {
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
    std::vector<Complex> graph;
    XmlElement *data = mainElement->getChildByName(Name);
    forEachXmlChildElement(*data, d)
    {
        graph.emplace_back(Complex((float)d->getDoubleAttribute("value"), (float)d->getDoubleAttribute("loc")));
    }
    std::vector<double> out;
    for (int i = 0; i < Size; i++)
    {
        out.emplace_back(Interpolate((double)i / (double)Size, graph));
    }
    return out;
}

std::vector<FFTW::AudioAnalysis> Settings::GetAudioData(String Name, int FFTSize) const
{
    DirectoryIterator iter(File(workingDir.getFullPathName() + "/Audio"), true, "*.aif");
    FFTW::AudioLoader loader(FFTSize);
    std::vector<FFTW::AudioAnalysis> out;
    while (iter.next())
    {
        File res(iter.getFile());
        out.emplace_back(loader.AnalyzeAudio(res, 1, 0));
    }
    return out;
}

std::vector<String> Settings::GetAudioGraph(String Name, int Size) const
{
    std::vector<String> out;
    std::vector<double> positions;
    std::vector<String> names;

    XmlElement *data = mainElement->getChildByName(Name);
    forEachXmlChildElement(*data, d)
    {
        positions.emplace_back(d->getDoubleAttribute("loc") * Size);
        names.emplace_back(d->getStringAttribute("value"));
    }

    int pos = 0;
    for (int i = 0; i < Size; i++)
    {
        if (pos < positions.size() - 1)
        {
            if (i >= positions[pos + 1])
                pos++;
        }
        out.emplace_back(names[pos]);
    }
    return out;
}

std::vector<String> Settings::GetAudioNameGraph(String Name) const
{
    std::vector<String> out;
    XmlElement *data = mainElement->getChildByName(Name);
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

void Settings::AddXmlElement(XmlElement *ToAdd)
{
    if (!mainElement->replaceChildElement(mainElement->getChildByName(ToAdd->getTagName()), ToAdd))
    {
        mainElement->addChildElement(ToAdd);
    }
}

const File &Settings::GetWorkingDirectory() const
{
    return workingDir;
}

void Settings::AddListener(Listener *l)
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

double Settings::Interpolate(double Position, std::vector<Complex> Data) const
{
    double output = Data[Data.size() - 1].Real;
    int i = 0;
    while (i < Data.size())
    {
        if (i == 0 && Position < Data[i].Imaginery)
            return Data[i].Real;
        else if (Position < Data[i].Imaginery)
        {
            double distance = (Position - Data[i - 1].Imaginery) / (Data[i].Imaginery - Data[i - 1].Imaginery);
            return LinearInterp(distance, Data[i - 1].Real, Data[i].Real);
        }
        else if (Position == Data[i].Imaginery)
        {
            return Data[i].Real;
        }
        i++;
    }
    return output;
}