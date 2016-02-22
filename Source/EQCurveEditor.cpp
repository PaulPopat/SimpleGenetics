/*
  ==============================================================================

    EQCurveEditor.cpp
    Created: 18 Feb 2016 3:52:50pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "EQCurveEditor.h"

EQCurveEditor::EQCurveEditor(String Title, String Name)
    : GraphEditor(Title, Name)
{
}

EQCurveEditor::~EQCurveEditor()
{
}

void EQCurveEditor::floorData()
{
    for (auto& d : data) {
        if (d.Value < 0)
            d.Value = 0;
        else if (d.Value > 1)
            d.Value = 1;
    }
}