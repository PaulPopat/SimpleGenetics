/*
  ==============================================================================

    ProgressBox.cpp
    Created: 9 Aug 2015 10:00:20am
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProgressBox.h"
using namespace std;
//==============================================================================
ProgressBox::ProgressBox(pair<double, double> Location, pair<double, double> Size, Colour FillColor, Colour LineColor) {
    location = Location;
    size = Size;
    display = new DrawBox(location, size);
    display->SetFillColor(FillColor);
    display->SetLineColor(LineColor);
    display->SetFontSize(size.second / 2);
    display->SetText("");
    color = LineColor;
}

ProgressBox::~ProgressBox() {
    delete display;
}

void ProgressBox::Draw(Graphics &g, double progress) {
    display->Draw(g);
    g.setColour(color);
    int position = (location.first - (size.first / 2)) + (size.first * progress);
    g.drawLine(position, location.second + (size.second / 2), position, location.second - (size.second / 2), 2);
}

void ProgressBox::SetName(String Name) {
    name = Name;
    display->SetText(name);
}

bool ProgressBox::PointInBox(pair<int, int> mPoint) {
    return display->PointIsInBox(mPoint);
}