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

TargetDisplay::TargetDisplay(int lx, int ly, int sx, int sy, Colour FillColor, Colour LineColor) {
    display = new ProgressBox(make_pair(lx, ly), make_pair(sx, sy), FillColor, LineColor);
    lineColor = Colour(255,255,255);
    size = make_pair(sx, sy);
    bLeftLoc = make_pair(lx - (sx / 2), ly - (sy / 2));
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

double TargetDisplay::Click(pair<int, int> MousePoint, bool Alt, bool Ctl) {
    if (Alt) {
        if (display->PointInBox(MousePoint)) {
            double x = ((double)MousePoint.first - (double)bLeftLoc.first) / (double)size.first;
            if (data[selectedPoint].second > 1)
                data[selectedPoint].second = 1;
            if (data[selectedPoint].second < 0)
                data[selectedPoint].second = 0;
            return x;
        }
    }
    for (int i = 0 ; i < data.size() ; i++) {
        int sx = (data[i].second * size.first) + bLeftLoc.first;
        if (MousePoint.first < sx + 4 &&
            MousePoint.first > sx - 4 &&
            MousePoint.second < bLeftLoc.second + size.second &&
            MousePoint.second > bLeftLoc.second) {
            if (Ctl) {
                data.erase(data.begin() + i);
                return -1;
            }
            selectedPoint = i;
            return -1;
        }
    }
    return -1;
}

void TargetDisplay::Drag(pair<int, int> MousePoint) {
    if (selectedPoint < 0)
        return;
    if (selectedPoint > 0) {
        data[selectedPoint].second = ((double)MousePoint.first - (double)bLeftLoc.first) / (double)size.first;
        if (data[selectedPoint].second > 1)
            data[selectedPoint].second = 1;
        if (data[selectedPoint].second < 0)
            data[selectedPoint].second = 0;
    }
}

vector<pair<string, double> > TargetDisplay::ClickUp() {
    sort(data.begin(), data.end(), Sorter);
    selectedPoint = -1;
    vector<pair<string, double> > retVal;
    for (int i = 0 ; i < data.size() ; i++) {
        retVal.push_back(make_pair(data[i].first.toRawUTF8(), data[i].second));
    }
    return retVal;
}

bool TargetDisplay::Sorter(const pair<String, double> &left, const pair<String, double> &right) {
    // sorts the closeness index
    return left.second < right.second;
}