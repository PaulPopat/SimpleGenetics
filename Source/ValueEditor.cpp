/*
  ==============================================================================

    ValueEditor.cpp
    Created: 18 Feb 2016 3:50:54pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ValueEditor.h"

ValueEditor::ValueEditor(String Title, String Name)
    : SettingsListenerComponent(Name)
{
    editor.addListener(this);
    editor.setText("No Settings!");
    editor.setReadOnly(true);
    editor.setFont(Font(12));
    addAndMakeVisible(editor);
    title = Title;
}

void ValueEditor::paint(Graphics& g)
{
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Text));
    g.setFont(12);
    g.drawText(title, titleBounds, Justification::centred);
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    g.drawRect(editorBounds, 2);
    g.drawRect(titleBounds, 2);
}

void ValueEditor::resized()
{
    bounds = Rectangle<int>(2, 2, getWidth() - 4, getHeight() - 4);
    titleBounds = Rectangle<int>(bounds.getX(), bounds.getY(), bounds.getWidth() / 2, bounds.getHeight());
    editorBounds = Rectangle<int>(titleBounds.getTopRight(), bounds.getBottomRight());
    editor.setBounds(editorBounds.reduced(2));
}

void ValueEditor::SettingsChanged(Settings* s)
{
    if (!s->IsLoaded()) {
        editor.setText("No Settings");
        editor.setReadOnly(true);
        return;
    }
    editor.setText(s->GetStringValue(elementName));
    editor.setReadOnly(false);
    repaint();
}

void ValueEditor::GetSettings(Settings* s)
{
    XmlElement* e = new XmlElement(elementName);
    e->setAttribute("value", editor.getText());
    s->AddXmlElement(e);
}

void ValueEditor::textEditorReturnKeyPressed(TextEditor&)
{
    unfocusAllComponents();
}

void ValueEditor::textEditorEscapeKeyPressed(TextEditor&)
{
    unfocusAllComponents();
}