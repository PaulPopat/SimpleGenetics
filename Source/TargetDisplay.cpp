/*
  ==============================================================================

    TargetDisplay.cpp
    Created: 9 Aug 2015 10:00:33am
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TargetDisplay.h"
using namespace std;

TargetDisplay::TargetDisplay(pair<double, double> Location, pair<double, double> Size, Colour FillColor, Colour LineColor) {
    display = new ProgressBox(Location, Size, FillColor, LineColor);
    lineColor = Colour(255,255,255);
    size = Size;
    bLeftLoc = make_pair(Location.first - (Size.first / 2), Location.second - (Size.second / 2));
}

TargetDisplay::~TargetDisplay() {
    delete display;
}

void TargetDisplay::Draw(Graphics &g, double Position) {
    display->Draw(g, Position);
    for (int i = 0 ; i < data.size() ; i++) {
        g.setColour(lineColor);
        int sx = (data[i].second * size.first) + bLeftLoc.first;
        int sy = bLeftLoc.second + size.second;
        int ey = bLeftLoc.second;
        g.drawLine(sx, sy, sx, ey, 2);
        g.setFont(8);
        g.drawSingleLineText(data[i].first, sx + 4, ey + 10);
    }
}

void TargetDisplay::LoadGraph(vector<pair<String, double> >Input) {
    data = Input;
}

void TargetDisplay::SetName(String Name) {
    display->SetName(Name);
}