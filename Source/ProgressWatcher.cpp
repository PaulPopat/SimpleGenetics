/*
  ==============================================================================

    ProgressWatcher.cpp
    Created: 18 Feb 2016 2:40:13pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProgressWatcher.h"

ProgressWatcher::ProgressWatcher(String Type)
{
    type = Type;
    setInterceptsMouseClicks(false, false);
}

ProgressWatcher::~ProgressWatcher()
{
}

void ProgressWatcher::paint(Graphics& g)
{
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::TextLow));
    g.setFont(32);
    g.drawFittedText(type, 2, 2, getWidth() - 4, getHeight() - 4, Justification::centred, 1);
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    g.drawRect(2, 2, getWidth() - 4, getHeight() - 4, 2);
    for (int i = 0; i < progress.size(); i++) {
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
        g.drawRect(bounds[i], 2);
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::Text));
        g.setFont(12);
        g.drawFittedText("Thread " + String(i + 1) + ": " + String(progress[i]), bounds[i], Justification::centred, 1);
    }
}

void ProgressWatcher::resized()
{
    SetUpBounds();
}

void ProgressWatcher::BreedComplete(const BreedData& data, const SettingsData & settings)
{
    if (settings.FrequencyBand >= progress.size()) {
        progress.resize(settings.FrequencyBand + 1);
        SetUpBounds();
    }
    if (type == "Frame")
        progress[settings.FrequencyBand] = data.Breed;
    else if (type == "Timbre")
        progress[settings.FrequencyBand] = data.TimbreMetric[0].Metric;
    else if (type == "Panning")
        progress[settings.FrequencyBand] = data.PanningMetric[0].Metric;
    triggerAsyncUpdate();
}

void ProgressWatcher::SetUpBounds()
{
    if (progress.size() == 0)
        return;
    int height = (getHeight() - 4) / progress.size();
    int width = getWidth() - 4;
    bounds.clear();
    for (int i = 0; i < progress.size(); i++) {
        bounds.emplace_back(Rectangle<int>(2, 2 + (height * i), width, height));
    }
}