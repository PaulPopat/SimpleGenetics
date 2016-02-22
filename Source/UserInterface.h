/*
  ==============================================================================

    UserInterface.h
    Created: 16 Feb 2016 11:51:56am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef USERINTERFACE_H_INCLUDED
#define USERINTERFACE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ListenerComponents.h"
#include "Settings.h"
#include "GeneController.h"
#include "FrequencyGraph.h"
#include "PanningDisplay.h"
#include "ProgressWatcher.h"
#include "ValueEditor.h"
#include "GraphEditor.h"
#include "EQCurveEditor.h"
#include "AudioBin.h"
#include "TargetEditor.h"
#include "ProgressLine.h"
#include "CustomLookAndFeel.h"


class UserInterface    : public Component
{
public:
    UserInterface(XmlElement * e, Settings * s);
    ~UserInterface();

    void paint (Graphics&);
    void resized();
    
    void LoadDisplay(XmlElement * e, GeneControllerListenerComponent * l);
    void LoadInterface(XmlElement * e, SettingsListenerComponent * l);
    void LoadTargetEditor(XmlElement * e, TargetEditor * l);
    void LoadAudioBin(XmlElement * e, AudioBin * l);
    void LoadGraphEditor(XmlElement * e, GraphEditor * l);
    
    void AddControllerListeners(GeneController * controller);
    void AddSettingsListeners(Settings * settings);

private:
    void LinkInternals();
    
    struct DisplayWithBounds {
        ScopedPointer<GeneControllerListenerComponent> Display;
        Rectangle<double> Bounds;
    };
    struct InterfaceWithBounds {
        ScopedPointer<SettingsListenerComponent> Interface;
        Rectangle<double> Bounds;
    };
    struct TargetEditorWithBounds {
        ScopedPointer<TargetEditor> Interface;
        Rectangle<double> Bounds;
    };
    struct AudioBinWithBounds {
        ScopedPointer<AudioBin> Interface;
        Rectangle<double> Bounds;
    };
    struct GraphEditorWithBounds {
        ScopedPointer<GraphEditor> Interface;
        Rectangle<double> Bounds;
    };
    
    Array<ScopedPointer<DisplayWithBounds>> displays;
    Array<ScopedPointer<InterfaceWithBounds>> interfaces;
    Array<ScopedPointer<AudioBinWithBounds>> audioBins;
    Array<ScopedPointer<TargetEditorWithBounds>> targetEditors;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UserInterface)
};


#endif  // USERINTERFACE_H_INCLUDED
