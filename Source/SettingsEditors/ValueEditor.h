/*
  ==============================================================================

    ValueEditor.h
    Created: 18 Feb 2016 3:50:54pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef VALUEEDITOR_H_INCLUDED
#define VALUEEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Settings/CustomLookAndFeel.h"
#include "../Utilities/ListenerComponents.h"
#include "../Settings/Settings.h"

class ValueEditor : public SettingsListenerComponent, public TextEditor::Listener
{
public:
  ValueEditor(String Title, String Name);

  void paint(Graphics &) override;
  void resized() override;

  void SettingsChanged(Settings *) override;
  void GetSettings(Settings *) override;

  void textEditorReturnKeyPressed(TextEditor &) override;
  void textEditorEscapeKeyPressed(TextEditor &) override;

private:
  TextEditor editor;
  String title;

  Rectangle<int> bounds;
  Rectangle<int> editorBounds;
  Rectangle<int> titleBounds;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueEditor)
};

#endif // VALUEEDITOR_H_INCLUDED
