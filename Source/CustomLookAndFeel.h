/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 19 Feb 2016 6:17:10pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef CUSTOMLOOKANDFEEL_H_INCLUDED
#define CUSTOMLOOKANDFEEL_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"

class CustomLookAndFeel : public LookAndFeel_V3 {
public:
    CustomLookAndFeel();
    void drawTickBox(Graphics& g, Component& component,
        float x, float y, float w, float h,
        bool ticked,
        bool isEnabled,
        bool isMouseOverButton,
        bool isButtonDown) override;

    enum ColourIDs {
        Line = 0x20000001,
        Outline = 0x20000002,
        Background = 0x20000003,
        TargetLow = 0x20000005,
        DataLow = 0x20000006,
        DataHigh = 0x20000007,
        Foreground = 0x20000008,
        TargetHigh = 0x20000009,
        Text = 0x2000000a,
        TextLow = 0x2000000b,
    };

private:
    Colour background;
    Colour outline;
    Colour line;
    Colour targetLow;
    Colour targetHigh;
    Colour dataLow;
    Colour dataHigh;
    Colour foreground;
    Colour text;
    Colour textLow;
    Colour richbackground;
};

#endif // CUSTOMLOOKANDFEEL_H_INCLUDED
