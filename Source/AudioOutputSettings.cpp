/*
  ==============================================================================

    AudioOutputSettings.cpp
    Created: 19 Feb 2016 2:34:54pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioOutputSettings.h"

AudioOutputSettings::AudioOutputSettings(File Input, File Output)
{
    bitDepth.setText("24");
    sampleRate.setText("44100");
    channels.setText("6");
    addAndMakeVisible(bitDepth);
    addAndMakeVisible(sampleRate);
    addAndMakeVisible(channels);
    
    goButton.setButtonText("Start");
    addAndMakeVisible(goButton);
    goButton.addListener(this);
    
    input = Input;
    output = Output;
}

AudioOutputSettings::~AudioOutputSettings()
{
}

void AudioOutputSettings::paint (Graphics& g)
{
    g.fillAll(findColour(CustomLookAndFeel::ColourIDs::Background));
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Text));
    g.drawText("Bit Depth", getWidth() / 2, 0, getWidth() / 2, getHeight() / 5, Justification::centred);
    g.drawText("Sample Rate", getWidth() / 2, getHeight() / 5, getWidth() / 2, getHeight() / 5, Justification::centred);
    g.drawText("Channel", getWidth() / 2, getHeight() / 5 * 2, getWidth() / 2, getHeight() / 5, Justification::centred);
}

void AudioOutputSettings::resized()
{
    bitDepth.setBounds(0, 0, getWidth() / 2, getHeight() / 5);
    sampleRate.setBounds(0, getHeight() / 5, getWidth() / 2, getHeight() / 5);
    channels.setBounds(0, getHeight() / 5 * 2, getWidth() / 2, getHeight() / 5);
    goButton.setBounds(0, getHeight() / 5 * 3, getWidth(), getHeight() / 5);
}

void AudioOutputSettings::buttonClicked(Button * b) {
    writer = new FFTW::AudioWriter(input, output,
                                   sampleRate.getText().getIntValue(),
                                   channels.getText().getIntValue(),
                                   bitDepth.getText().getIntValue());
    bar = new ProgressBar(writer->Progress);
    bar->setBounds(0, getHeight() / 5 * 4, getWidth(), getHeight() / 5);
    addAndMakeVisible(bar);
    writer->startThread();
}