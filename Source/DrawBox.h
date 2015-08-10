//
//  DrawBox.h
//  VirtualStage
//
//  Created by Paul Popat on 13/05/2015.
//
//

#ifndef DRAWBOX_H_INCLUDED
#define DRAWBOX_H_INCLUDED
#include <utility>
#include <iostream>
#include <cstring>
#include "../JuceLibraryCode/JuceHeader.h"

class DrawBox {
public:
    DrawBox(std::pair<double, double> Location, std::pair<double, double> Size);
    void Draw(Graphics &g);
    void SetFillColor(Colour Color);
    void SetLineColor(Colour Color);
    void SetText(String Text);
    void SetFontSize(int Size);
    std::pair<double, double> GetLocation();
    bool PointIsInBox(std::pair<double, double> Input);
private:
    float coords[4];
    Colour color = Colour(0, 0, 0);
    Colour lineColor = Colour(0, 0, 0);
    String text = "";
    Rectangle<int> box;
    int fontSize = 16;
protected:
    std::pair<double, double> location;
    std::pair<double, double> size;
};

#endif
