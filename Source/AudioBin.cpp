/*
  ==============================================================================

    AudioBin.cpp
    Created: 19 Feb 2016 11:43:53am
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioBin.h"

AudioBin::AudioBin(String Title, String Name)
    : SettingsListenerComponent(Name)
{
    table = new TableListBox(Title, this);
    table->setHeader(new TableHeaderComponent());
    table->getHeader().addColumn(Title, 1, 10);
    addAndMakeVisible(table);

    loadAudio.setButtonText("Load Audio");
    loadAudio.addListener(this);
    addAndMakeVisible(loadAudio);
}


void AudioBin::paint(Graphics& g)
{
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    g.drawRect(binbounds, 2);
    g.drawRect(menubounds, 2);
}

void AudioBin::resized()
{
    bounds = Rectangle<int>(2, 2, getWidth() - 4, getHeight() - 4);
    binbounds = Rectangle<int>(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight() / 8 * 7);
    menubounds = Rectangle<int>(binbounds.getBottomLeft(), bounds.getBottomRight());

    table->setBounds(binbounds.reduced(2));
    table->getHeader().setColumnWidth(1, table->getWidth());

    loadAudio.setBounds(menubounds.reduced(2));
}

void AudioBin::SettingsChanged(Settings* s)
{
    if (!s->IsLoaded())
        return;
    data = s->GetAudioBin();
    table->autoSizeAllColumns();
    table->updateContent();
    settings = s;
    repaint();
}

void AudioBin::GetSettings(Settings*) {}

int AudioBin::getNumRows()
{
    return data.size();
}

void AudioBin::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowNumber % 2 == 0)
        g.fillAll(findColour(CustomLookAndFeel::ColourIDs::Background));
    else
        g.fillAll(findColour(CustomLookAndFeel::ColourIDs::Foreground));
}

void AudioBin::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    else
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::Line));
    g.drawRect(0, 0, width, height, 2);

    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Text));
    g.setFont(16);
    g.drawText(data[rowNumber], 0, 0, width, height, Justification::centred);
}

void AudioBin::cellDoubleClicked(int rowNumber, int columnId, const MouseEvent& e)
{
    if (e.mods.isCtrlDown()) {
        if (Settings::WarningAccepted("Are you sure you want to delete this audio file?... \n It will remove it from your hard drive!")) {
            settings->DeleteAudio(data[rowNumber]);
            table->updateContent();
        }
    }
    else {
        editors.call(&TargetEditor::AddAudio, data[rowNumber]);
    }
}

void AudioBin::buttonClicked(Button* b)
{
    if (settings != nullptr) {
        settings->LoadAudio();
    }
}

void AudioBin::AddListener(TargetEditor* e)
{
    editors.add(e);
}