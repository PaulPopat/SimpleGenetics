/*
  ==============================================================================

    Settings.h
    Created: 5 Aug 2015 12:07:13pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioLoader.h"
#include <fstream>

class Settings {
public:
    Settings();

    class Listener {
    public:
        Listener(String ElementName) { elementName = ElementName; }
        virtual ~Listener() = default;
        virtual void SettingsChanged(Settings*) = 0;
        virtual void GetSettings(Settings*) = 0;
        virtual void SetElementName(String ElementName) { elementName = ElementName; }
        virtual String GetElementName() const { return elementName; }
    protected:
        String elementName;
    };

    void LoadSettings();
    void SaveSettings();
    void SaveSettingsAs();
    void CreateNew();
    void Reset();
    bool IsLoaded();

    void LoadAudio();
    void DeleteAudio(String Name);
    StringArray GetAudioBin();

    double GetDoubleValue(String Name) const;
    int GetIntValue(String Name) const;
    String GetStringValue(String Name) const;
    Array<double> GetGraph(String Name, int Size) const;
    Array<FFTW::AudioAnalysis> GetAudioData(String Name, int FFTSize, int NumBands, int Band) const;
    Array<int> GetAudioGraph(String Name, int Size) const;
    Array<String> GetAudioNameGraph(String Name) const;
    XmlElement GetRawData(String Name) const;

    void AddXmlElement(XmlElement* ToAdd);

    void AddListener(Listener* l);
    void UpdateFromUI();

    const File& GetWorkingDirectory() const;

    static bool WarningAccepted(String warning);

private:
    ScopedPointer<XmlElement> mainElement;
    File workingDir;
    File save;
    ListenerList<Listener> listeners;
    bool isLoaded;
    double Interpolate(double Position, Array<FFT::Complex> Data) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Settings)
};

#endif // SETTINGS_H_INCLUDED
