/*
  ==============================================================================

    AudioOutputSettings.h
    Created: 19 Feb 2016 2:34:54pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef AUDIOOUTPUTSETTINGS_H_INCLUDED
#define AUDIOOUTPUTSETTINGS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioWriter.h"
#include "CustomLookAndFeel.h"
#include "Settings.h"

class AudioOutputSettings : public Component, public Button::Listener {
public:
    AudioOutputSettings(File Input, File Output);
    ~AudioOutputSettings();

    void paint(Graphics&) override;
    void resized() override;

    void buttonClicked(Button*) override;

private:
    TextEditor bitDepth;
    TextEditor sampleRate;
    TextEditor channels;
    ScopedPointer<ProgressBar> bar;
    ScopedPointer<FFTW::AudioWriter> writer;
    File input;
    File output;

    TextButton goButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioOutputSettings)
};

#endif // AUDIOOUTPUTSETTINGS_H_INCLUDED
