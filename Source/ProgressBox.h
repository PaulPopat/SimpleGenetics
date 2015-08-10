/*
  ==============================================================================

    ProgressBox.h
    Created: 9 Aug 2015 10:00:20am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef PROGRESSBOX_H_INCLUDED
#define PROGRESSBOX_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "DrawBox.h"
#include <utility>

class ProgressBox
{
public:
    ProgressBox(std::pair<double, double> Location, std::pair<double, double> Size, Colour FillColor, Colour LineColor);
    ~ProgressBox();
    void Draw(Graphics &g, double progress);
    void SetName(String Name);
private:
    String name;
    Colour color;
    DrawBox *display;
    double position;
    std::pair<double, double> location;
    std::pair<double, double> size;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgressBox)
};


#endif  // PROGRESSBOX_H_INCLUDED
