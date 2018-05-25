/*
  ==============================================================================

    AudioBin.h
    Created: 19 Feb 2016 11:43:53am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef AUDIOBIN_H_INCLUDED
#define AUDIOBIN_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Settings/CustomLookAndFeel.h"
#include "../Utilities/ListenerComponents.h"
#include "../Settings/Settings.h"
#include "../SettingsEditors/TargetEditor.h"

class AudioBin : public SettingsListenerComponent, public TableListBoxModel, public Button::Listener
{
public:
  AudioBin(String Title, String Name);

  void paint(Graphics &) override;
  void resized() override;

  void SettingsChanged(Settings *) override;
  void GetSettings(Settings *) override;

  int getNumRows() override;
  void paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected) override;
  void paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
  void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent &) override;

  void buttonClicked(Button *) override;

  void AddListener(TargetEditor *e);

private:
  Rectangle<int> bounds;
  Rectangle<int> binbounds;
  Rectangle<int> menubounds;

  ScopedPointer<TableListBox> table;

  TextButton loadAudio;

  Settings *settings;

  ListenerList<TargetEditor> editors;

  StringArray data;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioBin)
};

#endif // AUDIOBIN_H_INCLUDED
