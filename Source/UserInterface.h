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
#include "SettingsEditors/AudioBin.h"
#include "Settings/CustomLookAndFeel.h"
#include "SettingsEditors/EQCurveEditor.h"
#include "ProgressUpdates/FrequencyGraph.h"
#include "Algorithm/GeneController.h"
#include "SettingsEditors/GraphEditor.h"
#include "Utilities/ListenerComponents.h"
#include "ProgressUpdates/PanningDisplay.h"
#include "ProgressUpdates/ProgressLine.h"
#include "ProgressUpdates/ProgressWatcher.h"
#include "Settings/Settings.h"
#include "SettingsEditors/TargetEditor.h"
#include "SettingsEditors/ValueEditor.h"

class UserInterface : public Component
{
  public:
    UserInterface(XmlElement *e, Settings *s);

    void paint(Graphics &) override;
    void resized() override;

    void LoadDisplay(XmlElement *e, GeneControllerListenerComponent *l);
    void LoadInterface(XmlElement *e, SettingsListenerComponent *l);
    void LoadTargetEditor(XmlElement *e, TargetEditor *l);
    void LoadAudioBin(XmlElement *e, AudioBin *l);
    void LoadGraphEditor(XmlElement *e, GraphEditor *l);

    void AddControllerListeners(GeneController *controller);
    void AddSettingsListeners(Settings *settings);

  private:
    void LinkInternals();

    struct DisplayWithBounds
    {
        ScopedPointer<GeneControllerListenerComponent> Display;
        Rectangle<double> Bounds;
    };
    struct InterfaceWithBounds
    {
        ScopedPointer<SettingsListenerComponent> Interface;
        Rectangle<double> Bounds;
    };
    struct TargetEditorWithBounds
    {
        ScopedPointer<TargetEditor> Interface;
        Rectangle<double> Bounds;
    };
    struct AudioBinWithBounds
    {
        ScopedPointer<AudioBin> Interface;
        Rectangle<double> Bounds;
    };
    struct GraphEditorWithBounds
    {
        ScopedPointer<GraphEditor> Interface;
        Rectangle<double> Bounds;
    };

    std::vector<ScopedPointer<DisplayWithBounds>> displays;
    std::vector<ScopedPointer<InterfaceWithBounds>> interfaces;
    std::vector<ScopedPointer<AudioBinWithBounds>> audioBins;
    std::vector<ScopedPointer<TargetEditorWithBounds>> targetEditors;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserInterface)
};

#endif // USERINTERFACE_H_INCLUDED
