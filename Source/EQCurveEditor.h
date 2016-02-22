/*
  ==============================================================================

    EQCurveEditor.h
    Created: 18 Feb 2016 3:52:50pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef EQCURVEEDITOR_H_INCLUDED
#define EQCURVEEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomLookAndFeel.h"
#include "GraphEditor.h"
#include "ListenerComponents.h"
#include "Settings.h"

class EQCurveEditor : public GraphEditor {
public:
    EQCurveEditor(String Title, String Name);
    ~EQCurveEditor();

private:
    void floorData() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQCurveEditor)
};

#endif // EQCURVEEDITOR_H_INCLUDED
