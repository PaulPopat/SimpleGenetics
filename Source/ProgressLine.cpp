/*
  ==============================================================================

    ProgressLine.cpp
    Created: 20 Feb 2016 10:34:31am
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProgressLine.h"

ProgressLine::ProgressLine(Settings* s)
{
    settings = s;
    setInterceptsMouseClicks(false, false);
}

ProgressLine::~ProgressLine()
{
}

void ProgressLine::paint(Graphics& g)
{
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Text));
    for (const auto& p : progress) {
        int pos = bounds.getX() + (p * bounds.getWidth());
        g.drawLine(pos, bounds.getY() + 2, pos, bounds.getBottom() - 2, 2);
    }
}

void ProgressLine::resized()
{
    bounds = Rectangle<int>(2, 2, getWidth() - 4, getHeight() - 4);
}

void ProgressLine::BreedComplete(const BreedCompleteData& data)
{
    if (data.ident >= progress.size())
        progress.resize(data.ident + 1);

    progress.getReference(data.ident) = std::fmod((double)data.framesComplete,
                                            settings->GetDoubleValue("CalculationLoops"))
        / settings->GetDoubleValue("BreedingLoops");
    triggerAsyncUpdate();
}