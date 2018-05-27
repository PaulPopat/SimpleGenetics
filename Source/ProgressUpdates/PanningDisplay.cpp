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

void PanningDisplay::paint(Graphics &g)
{
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    g.drawRect(bounds, 2);

    double s = getHeight();
    double w = (getWidth() - getHeight()) / 2;
    double h = (getHeight() - getWidth()) / 2;
    if (getWidth() < getHeight())
    {
        s = getWidth();
        w = 0;
    }
    else
        h = 0;

    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Foreground));
    Point<int> tPos((targets.real() * 0.9 + 1) * (s * 0.5 + w),
                    (targets.imag() * 0.9 + 1) * (s * 0.5 + h));
    g.fillRect(tPos.x, tPos.y, 6, 6);
    g.setColour(InterpolateColour(findColour(CustomLookAndFeel::ColourIDs::TargetLow),
                                  findColour(CustomLookAndFeel::ColourIDs::TargetHigh),
                                  0));
    g.drawRect(tPos.x, tPos.y, 6, 6, 1);

    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Foreground));
    Point<int> pos((positions.real() * 0.9 + 1) * (s * 0.5 + w),
                   (positions.imag() * 0.9 + 1) * (s * 0.5 + h));
    g.fillRect(pos.x, pos.y, 6, 6);
    g.setColour(InterpolateColour(findColour(CustomLookAndFeel::ColourIDs::DataLow),
                                  findColour(CustomLookAndFeel::ColourIDs::DataHigh),
                                  0));
    g.drawRect(pos.x, pos.y, 6, 6, 1);
}

void PanningDisplay::resized()
{
    bounds = Rectangle<int>(2, 2, getWidth() - 4, getHeight() - 4);
}

void PanningDisplay::BreedComplete(const BreedData &d, const SettingsData &s)
{
    positions = d.Metrics[0].Panning.GetLocation();
    targets = d.CurrentTarget.Position;
    triggerAsyncUpdate();
}

Colour PanningDisplay::InterpolateColour(Colour c1, Colour c2, double progress)
{
    return Colour(((c2.getFloatRed() - c1.getFloatRed()) * progress + c1.getFloatRed()) * 255,
                  ((c2.getFloatGreen() - c1.getFloatGreen()) * progress + c1.getFloatGreen()) * 255,
                  ((c2.getFloatBlue() - c1.getFloatBlue()) * progress + c1.getFloatBlue()) * 255);
}