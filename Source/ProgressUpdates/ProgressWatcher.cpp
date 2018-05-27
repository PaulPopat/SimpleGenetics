/*
  ==============================================================================

    ProgressWatcher.cpp
    Created: 18 Feb 2016 2:40:13pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProgressWatcher.h"

ProgressWatcher::ProgressWatcher(String type)
{
    this->type = type;
    setInterceptsMouseClicks(false, false);
}

ProgressWatcher::~ProgressWatcher()
{
}

void ProgressWatcher::paint(Graphics &g)
{
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    const auto bounds = Rectangle<int>(2, 2, getWidth() - 4, getHeight() - 4);
    g.drawRect(bounds, 2);
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Text));
    g.setFont(12);
    g.drawFittedText(this->type + ": " + String(this->progress), bounds, Justification::centred, 1);
}

void ProgressWatcher::BreedComplete(const BreedData &data, const SettingsData &settings)
{
    if (this->type == "Frame")
        this->progress = data.Breed;
    else if (this->type == "Progress")
        this->progress = data.Metrics[0].Value;
    triggerAsyncUpdate();
}