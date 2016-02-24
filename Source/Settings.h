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

class Settings {
public:
    /** initialises settings with nothing. you will need to load or create to be able to do anything */
    Settings();

    /** listener class to hear when settings have been changed or loaded as well as
     apply settings when asked */
    class Listener {
    public:
        /** a way of storing the name of the setting this wants to edit */
        Listener(String ElementName) { elementName = ElementName; }
        virtual ~Listener() = default;
        /** called when new settings are load/created etc. */
        virtual void SettingsChanged(Settings*) = 0;
        /** called when the settings want any updates to run the algorithm or save */
        virtual void GetSettings(Settings*) = 0;
        /** another way of settings the name of the setting this wants to edit */
        virtual void SetElementName(String ElementName) { elementName = ElementName; }
        /** a way of getting what this wants to work with */
        virtual String GetElementName() const { return elementName; }
    protected:
        /** the name of the setting that this wants to edit */
        String elementName;
    };

    /** prompts the user to load settings from an xml file */
    void LoadSettings();
    /** prompts the user to save settings to the current xml file */
    void SaveSettings();
    /** prompts the user to save settings to an new xml file 
     (will also create directories for audio etc.) */
    void SaveSettingsAs();
    /** prompts the user to create new settings to an new xml file 
     (will also create directories for audio etc.) */
    void CreateNew();
    /** will reset the settings to unloaded only really useful to the class
     but may have some uses */
    void Reset();
    /** returns if settings are currently loaded. useful for locking down
     the ui if settings aren't loaded */
    bool IsLoaded();

    /** will load audio into the audio directory to be referenced by the audio bin and
     to be used as targets in the target editor */
    void LoadAudio();
    /** will remove an audio file from the audio directory */
    void DeleteAudio(String Name);
    /** returns a list of all the audio files in the audio director */
    StringArray GetAudioBin();

    /** returns the settings with given name as a double value */
    double GetDoubleValue(String Name) const;
    /** returns the settings with given name as an int value */
    int GetIntValue(String Name) const;
    /** returns the settings with given name as a string value */
    String GetStringValue(String Name) const;
    /** returns the settings with given name as a graph with levels for each
     point in the given size*/
    Array<double> GetGraph(String Name, int Size) const;
    /** returns the analysis of each audio file in the targets in order
     call get audio graph to find out when to look at each target */
    Array<FFTW::AudioAnalysis> GetAudioData(String Name, int FFTSize, int NumBands, int Band) const;
    /** returns the index in the GetAudioData return that needs to be looked at
     at any point in time */
    Array<int> GetAudioGraph(String Name, int Size) const;
    /** returns the name of the file in the GetAudioData return that needs to be looked at
     at any point in time */
    Array<String> GetAudioNameGraph(String Name) const;
    /** returns a copy of the xml data for a settings of given name. usefull for the settings
     editors to make adjustments and then return a new version when asked */
    XmlElement GetRawData(String Name) const;

    /** adds an xml element to the settings. if one of the same name exists it replaces it.
     useful for settings editors to put their adjustments back in */
    void AddXmlElement(XmlElement* ToAdd);

    /** adds a listener to get changes and apply their settings as needed */
    void AddListener(Listener* l);
    /** calls each of the listeners to apply their changes */
    void UpdateFromUI();

    /** returns a reference to the directory that the settings are being loaded from */
    const File& GetWorkingDirectory() const;

    /** displays a warning with given message that the user can accept or decline.
     returns true or false respectively */
    static bool WarningAccepted(String warning);

private:
    ScopedPointer<XmlElement> mainElement;
    File workingDir;
    File save;
    ListenerList<Listener> listeners;
    bool isLoaded;
    /** interpolates through a graph array. this is for the get graph method to work out
     what the value should be at any given time from a set of coordinates. */
    double Interpolate(double Position, Array<FFT::Complex> Data) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Settings)
};

#endif // SETTINGS_H_INCLUDED
