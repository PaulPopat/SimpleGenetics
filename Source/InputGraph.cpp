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

InputGraph::InputGraph(int lx, int ly, int sx, int sy, Colour FillColor, Colour LineColor, TextEditor *HighVal) {
    mainDisplay = new ProgressBox(make_pair(lx, ly), make_pair(sx, sy), FillColor, LineColor);
    pair<int, int> boxSize = make_pair(sx / 6, sy / 4);
    int xloc = (lx + (sx / 2)) - boxSize.first - 2;
    int yloc = (ly + (sy / 2)) - boxSize.second - 2;
    
    highVal = HighVal;
    highVal->setMultiLine(false);
    highVal->setCentrePosition(xloc, yloc);
    highVal->setSize(boxSize.first, boxSize.second);
    highVal->setText(to_string(highPoint));
    
    size = make_pair(sx, sy);
    bLeftLoc = make_pair(lx - (size.first / 2), ly - (size.second / 2));
    graphColor = Colour(255,255,255);
}

InputGraph::~InputGraph() {
    delete mainDisplay;
    delete highVal;
}

void InputGraph::Draw(Graphics &g, double progress) {
    vector<pair<double, double> > toDraw;
    for (int i = 0 ; i < data.size() ; i++) {
        toDraw.push_back(data[i]);
    }
    sort(toDraw.begin(), toDraw.end(), Sorter);
    mainDisplay->Draw(g, progress);
    for (int i = 0 ; i < data.size() - 1 ; i++) {
        DrawLine(g, toDraw[i], toDraw[i + 1]);
    }
    DrawLine(g, toDraw[data.size() - 1], make_pair(toDraw[data.size() - 1].first, 1));
}

void InputGraph::LoadGraph(vector<pair<double, double> >Input) {
    data = Input;
    highPoint = 0;
    for (int i = 0 ; i < data.size(); i++) {
        if (highPoint < data[i].first) highPoint = data[i].first + (data[i].first / 4);
    }
    highVal->setText(to_string(highPoint));
}

void InputGraph::SetName(juce::String Name) {
    mainDisplay->SetName(Name);
}

void InputGraph::textEditorReturnKeyPressed(TextEditor &editor) {
    String text = editor.getText();
    if (atof(text.getCharPointer())) {
        highPoint = atof(text.getCharPointer());
    }
    editor.setText(to_string(highPoint));
}

void InputGraph::textEditorEscapeKeyPressed(TextEditor &editor) {
    editor.setText(to_string(highPoint));
}

void InputGraph::Click(pair<int, int> MousePoint, bool Alt, bool Ctl) {
    for (int i = 0 ; i < data.size() ; i++) {
        int sx = (data[i].second * size.first) + bLeftLoc.first;
        int sy = (bLeftLoc.second + size.second) - ((data[i].first / highPoint) * size.second);
        if (MousePoint.first < sx + 4 &&
            MousePoint.first > sx - 4 &&
            MousePoint.second < sy + 4 &&
            MousePoint.second > sy - 4) {
            if (Ctl) {
                data.erase(data.begin() + i);
                return;
            }
            selectedPoint = i;
            return;
        }
    }
    if (Alt) {
        if (!mainDisplay->PointInBox(MousePoint)) {
            return;
        }
        double x = ((double)MousePoint.second - (double)bLeftLoc.second) / (double)size.second;
        x = 1 - x;
        x *= highPoint;
        double y = ((double)MousePoint.first - (double)bLeftLoc.first) / (double)size.first;
        data.push_back(make_pair(x, y));
        selectedPoint = data.size() - 1;
    }
}

void InputGraph::Drag(pair<int, int> MousePoint) {
    if (selectedPoint < 0)
        return;
    if (selectedPoint > 0) {
        data[selectedPoint].second = ((double)MousePoint.first - (double)bLeftLoc.first) / (double)size.first;
        if (data[selectedPoint].second > 1)
            data[selectedPoint].second = 1;
        if (data[selectedPoint].second < 0)
            data[selectedPoint].second = 0;
    }
    data[selectedPoint].first = ((double)MousePoint.second - (double)bLeftLoc.second) / (double)size.second;
    if (data[selectedPoint].first > 1)
        data[selectedPoint].first = 1;
    if (data[selectedPoint].first < 0)
        data[selectedPoint].first = 0;
    data[selectedPoint].first = 1 - data[selectedPoint].first;
    data[selectedPoint].first *= highPoint;
}

vector<pair<double, double> > InputGraph::ClickUp() {
    sort(data.begin(), data.end(), Sorter);
    selectedPoint = -1;
    return data;
}

bool InputGraph::Sorter(const pair<double, double> &left, const pair<double, double> &right) {
    // sorts the closeness index
    return left.second < right.second;
}