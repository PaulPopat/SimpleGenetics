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
    TargetDisplay(int lx, int ly, int sx, int sy, Colour FillColor, Colour LineColor);
    ~TargetDisplay();
    void Draw(Graphics &g, double Position);
    void LoadGraph(std::vector<std::pair<String, double> >Input);
    void SetName(String Name);
    double Click(std::pair<int,int> MousePoint, bool Alt, bool Ctl);
    void Drag(std::pair<int, int> MousePoint);
    std::vector<std::pair<std::string, double> > ClickUp();
private:
    int selectedPoint = -1;
    std::pair<int,int> size;
    std::pair<int,int> bLeftLoc;
    std::vector<std::pair<String, double> > data;
    ProgressBox* display;
    Colour lineColor;
    
    static bool Sorter(const std::pair<String, double> &left, const std::pair<String, double> &right);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TargetDisplay)
};


#endif  // TARGETDISPLAY_H_INCLUDED
