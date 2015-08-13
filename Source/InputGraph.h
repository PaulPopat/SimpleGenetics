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
#include <vector>
#include <utility>

class InputGraph {
public:
    InputGraph(int lx, int ly, int sx, int sy, Colour FillColor, Colour LineColor, TextEditor *HighVal);
    ~InputGraph();
    void Draw(Graphics &g, double progress);
    void LoadGraph(std::vector<std::pair<double, double> >Input);
    void SetName(String Name);
    std::vector<std::pair<double, double> > textEditorReturnKeyPressed(TextEditor &editor);
    void textEditorEscapeKeyPressed(TextEditor &editor);
    void Click(std::pair<int,int> MousePoint, bool Alt, bool Ctl);
    void Drag(std::pair<int, int> MousePoint);
    std::vector<std::pair<double, double> > ClickUp();
private:
    std::pair<int,int> size;
    std::pair<int,int> bLeftLoc;
    double highPoint = 1;
    ProgressBox* mainDisplay;
    TextEditor* highVal;
    int selectedPoint = -1;
    std::vector<std::pair<double, double> > data;
    Colour graphColor;
    static bool Sorter(const std::pair<double, double> &left, const std::pair<double, double> &right);
    
    void DrawLine(Graphics &g, std::pair<double, double> v1, std::pair<double, double> v2);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputGraph)
};


#endif  // INPUTGRAPH_H_INCLUDED
