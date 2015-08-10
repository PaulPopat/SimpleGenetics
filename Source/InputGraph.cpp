/*
  ==============================================================================

    InputGraph.cpp
    Created: 5 Aug 2015 4:29:57pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "InputGraph.h"
using namespace std;

void InputGraph::DrawLine(Graphics &g, pair<double, double> v1, pair<double, double> v2) {
    g.setColour(graphColor);
    // draw the lines for thte graph
    int sx = (v1.second * size.first) + bLeftLoc.first;
    int sy = (bLeftLoc.second + size.second) - ((v1.first / highPoint) * size.second);
    int ex = (v2.second * size.first) + bLeftLoc.first;
    int ey = (bLeftLoc.second + size.second) - ((v2.first / highPoint) * size.second);
    g.drawLine(sx, sy, ex, ey, 2);
    // draw the boxes to mark changes
    Rectangle<int> start = Rectangle<int>(Point<int>(sx - 2, sy - 2), Point<int>(sx + 2, sy + 2));
    g.fillRect(start);
}

InputGraph::InputGraph(pair<double,double> Location,
                          pair<double,double> Size,
                          Colour FillColor,
                          Colour LineColor) {
    
    mainDisplay = new ProgressBox(Location, Size, FillColor, LineColor);
    pair<int, int> boxSize = make_pair(Size.first / 8, Size.second / 8);
    int offset = Size.first / 2;
    int xloc = (Location.first + offset) - (boxSize.first / 2) - 2;
    int yoffset = (Size.second / 2) - (boxSize.second / 2) - 2;
    lowVal = new DrawBox(make_pair(xloc, Location.second + yoffset), boxSize);
    highVal = new DrawBox(make_pair(xloc, Location.second - yoffset), boxSize);
    
    lowVal->SetFillColor(Colour((uint8)0, (uint8)0, (uint8)0, (uint8)0));
    lowVal->SetLineColor(LineColor);
    lowVal->SetFontSize(8);
    lowVal->SetText("High");
    
    highVal->SetFillColor(Colour((uint8)0, (uint8)0, (uint8)0, (uint8)0));
    highVal->SetLineColor(LineColor);
    highVal->SetFontSize(8);
    highVal->SetText("Low");
    
    size = Size;
    bLeftLoc = make_pair(Location.first - (Size.first / 2), Location.second - (Size.second / 2));
    graphColor = Colour(255,255,255);
}

InputGraph::~InputGraph() {
    delete mainDisplay;
    delete highVal;
    delete lowVal;
}

void InputGraph::Draw(Graphics &g, double progress) {
    mainDisplay->Draw(g, progress);
    for (int i = 0 ; i < data.size() - 1 ; i++) {
        DrawLine(g, data[i], data[i + 1]);
    }
    DrawLine(g, data[data.size() - 1], make_pair(data[data.size() - 1].first, 1));
    highVal->Draw(g);
    lowVal->Draw(g);
}

void InputGraph::LoadGraph(std::vector<std::pair<double, double> >Input) {
    data = Input;
    highPoint = 0;
    lowVal->SetText(to_string(0));
    for (int i = 0 ; i < data.size(); i++) {
        if (highPoint < data[i].first) highPoint = data[i].first + (data[i].first / 4);
    }
    highVal->SetText(to_string(highPoint));
}

void InputGraph::SetName(juce::String Name) {
    mainDisplay->SetName(Name);
}