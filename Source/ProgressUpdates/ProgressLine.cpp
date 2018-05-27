/*
  ==============================================================================

    ProgressLine.cpp
    Created: 20 Feb 2016 10:34:31am
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProgressLine.h"

ProgressLine::ProgressLine(Settings *s)
{
    settings = s;
    setInterceptsMouseClicks(false, false);
}

ProgressLine::~ProgressLine()
{
}

void ProgressLine::paint(Graphics &g)
{
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Text));
    int pos = bounds.getX() + (this->progress * bounds.getWidth());
    g.drawLine(pos, bounds.getY() + 2, pos, bounds.getBottom() - 2, 2);
}

void ProgressLine::resized()
{
    bounds = Rectangle<int>(2, 2, getWidth() - 4, getHeight() - 4);
}

void ProgressLine::BreedComplete(const BreedData &d, const SettingsData &s)
{
    this->progress = std::fmod((double)d.Breed / settings->GetDoubleValue("BreedingLoops"),
                               settings->GetDoubleValue("CalculationLoops"));
    triggerAsyncUpdate();
}