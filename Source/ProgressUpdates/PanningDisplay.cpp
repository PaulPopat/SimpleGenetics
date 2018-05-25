/*
  ==============================================================================

    PanningDisplay.cpp
    Created: 16 Feb 2016 9:54:26am
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PanningDisplay.h"

PanningDisplay::PanningDisplay()
{
    setInterceptsMouseClicks(false, false);
}

void PanningDisplay::paint(Graphics& g)
{
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    g.drawRect(bounds, 2);

    double s = getHeight();
    double w = (getWidth() - getHeight()) / 2;
    double h = (getHeight() - getWidth()) / 2;
    if (getWidth() < getHeight()) {
        s = getWidth();
        w = 0;
    }
    else
        h = 0;

    for (int i = 0; i < targets.size(); i++) {
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::Foreground));
        Point<int> pos((targets[i].real() * 0.9 + 1) * (s * 0.5 + w),
            (targets[i].imag() * 0.9 + 1) * (s * 0.5 + h));
        g.fillRect(pos.x, pos.y, 6, 6);
        g.setColour(InterpolateColour(findColour(CustomLookAndFeel::ColourIDs::TargetLow),
            findColour(CustomLookAndFeel::ColourIDs::TargetHigh), i / targets.size()));
        g.drawRect(pos.x, pos.y, 6, 6, 1);
    }

    for (int i = 0; i < positions.size(); i++) {
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::Foreground));
        Point<int> pos((positions[i].real() * 0.9 + 1) * (s * 0.5 + w),
            (positions[i].imag() * 0.9 + 1) * (s * 0.5 + h));
        g.fillRect(pos.x, pos.y, 6, 6);
        g.setColour(InterpolateColour(findColour(CustomLookAndFeel::ColourIDs::DataLow),
            findColour(CustomLookAndFeel::ColourIDs::DataHigh), i / positions.size()));
        g.drawRect(pos.x, pos.y, 6, 6, 1);
    }
}

void PanningDisplay::resized()
{
    bounds = Rectangle<int>(2, 2, getWidth() - 4, getHeight() - 4);
}

void PanningDisplay::BreedComplete(const BreedData& d, const SettingsData & s)
{
    if (s.FrequencyBand + 1 > positions.size())
        positions.resize(s.FrequencyBand + 1);
    if (s.FrequencyBand + 1 > targets.size())
        targets.resize(s.FrequencyBand + 1);
    positions[s.FrequencyBand] = d.PanningMetric[0].Gene.GetLocation();
    targets[s.FrequencyBand] = d.CurrentTarget.Position;
    triggerAsyncUpdate();
}

Colour PanningDisplay::InterpolateColour(Colour c1, Colour c2, double progress)
{
    return Colour(((c2.getFloatRed() - c1.getFloatRed()) * progress + c1.getFloatRed()) * 255,
        ((c2.getFloatGreen() - c1.getFloatGreen()) * progress + c1.getFloatGreen()) * 255,
        ((c2.getFloatBlue() - c1.getFloatBlue()) * progress + c1.getFloatBlue()) * 255);
}