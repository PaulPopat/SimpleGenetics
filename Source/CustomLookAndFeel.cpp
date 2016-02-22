/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 19 Feb 2016 6:17:10pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel() {
    background = Colour(20, 20, 20);
    outline = Colour(180, 180, 180);
    line = Colour(240, 240, 240);
    targetLow = Colour(120, 20, 20);
    targetHigh = Colour(230, 20, 90);
    dataLow = Colour(180, 120, 20);
    dataHigh = Colour(255, 220, 90);
    foreground = Colour(80, 80, 80);
    text = Colour(255, 255, 180);
    textLow = Colour(90, 90, 90);
    richbackground = Colour(40, 40, 40);
    
    setColour(ColourIDs::Background, background);
    setColour(ColourIDs::Outline, outline);
    setColour(ColourIDs::Line, line);
    setColour(ColourIDs::TargetLow, targetLow);
    setColour(ColourIDs::TargetHigh, targetHigh);
    setColour(ColourIDs::DataLow, dataLow);
    setColour(ColourIDs::DataHigh, dataHigh);
    setColour(ColourIDs::Foreground, foreground);
    setColour(ColourIDs::Text, text);
    setColour(ColourIDs::TextLow, textLow);
    setColour(TextEditor::ColourIds::backgroundColourId, richbackground);
    setColour(TextEditor::ColourIds::focusedOutlineColourId, text);
    setColour(TextEditor::ColourIds::highlightColourId, foreground);
    setColour(TextEditor::ColourIds::highlightedTextColourId, Colours::white);
    setColour(TextEditor::ColourIds::outlineColourId, outline);
    setColour(TextEditor::ColourIds::shadowColourId, background);
    setColour(TextEditor::ColourIds::textColourId, text);
    setColour(TableListBox::ColourIds::backgroundColourId, background);
    setColour(TableListBox::ColourIds::outlineColourId, outline);
    setColour(TableListBox::ColourIds::textColourId, Colours::white);
    setColour(TextButton::ColourIds::buttonColourId, richbackground);
    setColour(TextButton::ColourIds::buttonOnColourId, Colours::white);
    setColour(TextButton::ColourIds::textColourOffId, text);
    setColour(TextButton::ColourIds::textColourOnId, foreground);
    setColour(ProgressBar::ColourIds::backgroundColourId, foreground);
    setColour(ProgressBar::ColourIds::foregroundColourId, text);
    setColour(PopupMenu::ColourIds::backgroundColourId, richbackground);
    setColour(PopupMenu::ColourIds::headerTextColourId, text);
    setColour(PopupMenu::ColourIds::highlightedBackgroundColourId, foreground);
    setColour(PopupMenu::ColourIds::highlightedTextColourId, outline);
    setColour(PopupMenu::ColourIds::textColourId, text);
    setColour(ComboBox::ColourIds::arrowColourId, text);
    setColour(ComboBox::ColourIds::backgroundColourId, richbackground);
    setColour(ComboBox::ColourIds::buttonColourId, textLow);
    setColour(ComboBox::ColourIds::outlineColourId, outline);
    setColour(ComboBox::ColourIds::textColourId, text);
    setColour(Label::ColourIds::textColourId, text);
    setColour(ListBox::ColourIds::textColourId, text);
}

void CustomLookAndFeel::drawTickBox (Graphics& g, Component& component,
                  float x, float y, float w, float h,
                  bool ticked,
                  bool isEnabled,
                  bool isMouseOverButton,
                  bool isButtonDown) {
    if (isMouseOverButton) g.setColour(findColour(ColourIDs::Line));
    else g.setColour(findColour(ColourIDs::Outline));
    g.drawRect((int)x, (int)y, (int)w, (int)h, 2);
    
    if (ticked || isButtonDown) {
        if (ticked) g.setColour(findColour(ColourIDs::Text));
        else g.setColour(findColour(ColourIDs::DataLow));
        g.fillRect((int)x + 2, (int)y + 2, (int)w - 4, (int)h - 4);
    }
}