/*
  ==============================================================================

    TargetDisplay.h
    Created: 9 Aug 2015 10:00:33am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef TARGETDISPLAY_H_INCLUDED
#define TARGETDISPLAY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProgressBox.h"
#include "DrawBox.h"
#include <vector>
#include <utility>
class TargetDisplay
{
public:
    TargetDisplay(std::pair<double, double> Location, std::pair<double, double> Size, Colour FillColor, Colour LineColor);
    ~TargetDisplay();
    void Draw(Graphics &g, double Position);
    void LoadGraph(std::vector<std::pair<String, double> >Input);
    void SetName(String Name);
private:
    std::pair<int,int> size;
    std::pair<int,int> bLeftLoc;
    std::vector<std::pair<String, double> > data;
    ProgressBox* display;
    Colour lineColor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TargetDisplay)
};


#endif  // TARGETDISPLAY_H_INCLUDED
