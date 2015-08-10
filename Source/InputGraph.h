/*
  ==============================================================================

    InputGraph.h
    Created: 5 Aug 2015 4:29:57pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef INPUTGRAPH_H_INCLUDED
#define INPUTGRAPH_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProgressBox.h"
#include "DrawBox.h"
#include <vector>
#include <utility>

class InputGraph {
public:
    InputGraph(std::pair<double, double> Location, std::pair<double, double> Size, Colour FillColor, Colour LineColor);
    ~InputGraph();
    void Draw(Graphics &g, double progress);
    void LoadGraph(std::vector<std::pair<double, double> >Input);
    void SetName(String Name);
private:
    std::pair<int,int> size;
    std::pair<int,int> bLeftLoc;
    double highPoint = 1;
    ProgressBox* mainDisplay;
    DrawBox* highVal;
    DrawBox* lowVal;
    std::vector<std::pair<double, double> > data;
    Colour graphColor;
    
    void DrawLine(Graphics &g, std::pair<double, double> v1, std::pair<double, double> v2);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputGraph)
};


#endif  // INPUTGRAPH_H_INCLUDED
