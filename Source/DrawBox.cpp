//
//  DrawBox.cpp
//  VirtualStage
//
//  Created by Paul Popat on 13/05/2015.
//
//

#include "DrawBox.h"
using namespace std;

DrawBox::DrawBox(pair<double, double> Location, pair<double, double> Size) {
    size = Size;
    location = Location;
    coords[0] = location.first - (size.first / 2);
    coords[1] = location.second - (size.second / 2);
    coords[2] = location.first + (size.first / 2);
    coords[3] = location.second + (size.second / 2);
    
    Point<int> point[2];
    
    point[0].setXY(coords[0], coords[1]);
    point[1].setXY(coords[2], coords[3]);
    box = Rectangle<int>(point[0], point[1]);
}

void DrawBox::Draw(Graphics &g) {
    g.setColour(color);
    g.fillRect(box);
    g.setColour(lineColor);
    g.drawRect(box);
    g.setFont(fontSize);
    g.drawFittedText(text, box, Justification::centred, 1);
}

void DrawBox::SetFillColor(Colour Color) {
    color = Color;
}

void DrawBox::SetLineColor(Colour Color) {
    lineColor = Color;
}

void DrawBox::SetText(juce::String Text) {
    text = Text;
}

void DrawBox::SetFontSize(int Size) {
    fontSize = Size;
}

bool DrawBox::PointIsInBox(pair<double, double> Input) {
    if(Input.first > coords[0] &&
       Input.second > coords[1] &&
       Input.first < coords[2] &&
       Input.second < coords[3]) {
        return true;
    }
    return false;
}

pair<double, double> DrawBox::GetLocation() {return location;}