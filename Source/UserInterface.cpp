/*
  ==============================================================================

    UserInterface.cpp
    Created: 16 Feb 2016 11:51:56am
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "UserInterface.h"

//==============================================================================
UserInterface::UserInterface(XmlElement* e, Settings* s)
{
    forEachXmlChildElement(*e, c)
    {
        if (c->getTagName() == "FrequencyGraph")
            LoadDisplay(c, new FrequencyGraph());
        else if (c->getTagName() == "PanningDisplay")
            LoadDisplay(c, new PanningDisplay());
        else if (c->getTagName() == "ProgressWatcher")
            LoadDisplay(c, new ProgressWatcher(c->getStringAttribute("type")));
        else if (c->getTagName() == "ProgressLine")
            LoadDisplay(c, new ProgressLine(s));

        else if (c->getTagName() == "ValueEditor")
            LoadInterface(c, new ValueEditor(c->getStringAttribute("title"),
                                 c->getStringAttribute("track")));
        else if (c->getTagName() == "GraphEditor")
            LoadInterface(c, new GraphEditor(c->getStringAttribute("title"),
                                 c->getStringAttribute("track")));
        else if (c->getTagName() == "EQCurveEditor")
            LoadInterface(c, new EQCurveEditor(c->getStringAttribute("title"),
                                 c->getStringAttribute("track")));
        else if (c->getTagName() == "AudioBin")
            LoadAudioBin(c, new AudioBin(c->getStringAttribute("title"),
                                c->getStringAttribute("track")));
        else if (c->getTagName() == "TargetEditor")
            LoadTargetEditor(c, new TargetEditor(c->getStringAttribute("title"),
                                    c->getStringAttribute("track")));
    }
    LinkInternals();
}

void UserInterface::paint(Graphics& g)
{
    g.fillAll(findColour(CustomLookAndFeel::ColourIDs::Background));
}

void UserInterface::resized()
{
    for (auto& d : displays) {
        d->Display->setBounds(d->Bounds.getX() * getWidth(),
            d->Bounds.getY() * getHeight(),
            d->Bounds.getWidth() * getWidth(),
            d->Bounds.getHeight() * getHeight());
    }

    for (auto& i : interfaces) {
        i->Interface->setBounds(i->Bounds.getX() * getWidth(),
            i->Bounds.getY() * getHeight(),
            i->Bounds.getWidth() * getWidth(),
            i->Bounds.getHeight() * getHeight());
    }
    for (auto& i : targetEditors) {
        i->Interface->setBounds(i->Bounds.getX() * getWidth(),
            i->Bounds.getY() * getHeight(),
            i->Bounds.getWidth() * getWidth(),
            i->Bounds.getHeight() * getHeight());
    }
    for (auto& i : audioBins) {
        i->Interface->setBounds(i->Bounds.getX() * getWidth(),
            i->Bounds.getY() * getHeight(),
            i->Bounds.getWidth() * getWidth(),
            i->Bounds.getHeight() * getHeight());
    }
}

void UserInterface::LoadDisplay(XmlElement* e, GeneControllerListenerComponent* l)
{
    addAndMakeVisible(l);
    DisplayWithBounds* input = new DisplayWithBounds;
    input->Display = l;
    input->Bounds = Rectangle<double>(e->getDoubleAttribute("left"),
        e->getDoubleAttribute("top"),
        e->getDoubleAttribute("width"),
        e->getDoubleAttribute("height"));
    displays.emplace_back(input);
}

void UserInterface::LoadInterface(XmlElement* e, SettingsListenerComponent* l)
{
    addAndMakeVisible(l);
    InterfaceWithBounds* input = new InterfaceWithBounds;
    input->Interface = l;
    input->Bounds = Rectangle<double>(e->getDoubleAttribute("left"),
        e->getDoubleAttribute("top"),
        e->getDoubleAttribute("width"),
        e->getDoubleAttribute("height"));
    interfaces.emplace_back(input);
}

void UserInterface::LoadTargetEditor(XmlElement* e, TargetEditor* l)
{
    addAndMakeVisible(l);
    TargetEditorWithBounds* input = new TargetEditorWithBounds;
    input->Interface = l;
    input->Bounds = Rectangle<double>(e->getDoubleAttribute("left"),
        e->getDoubleAttribute("top"),
        e->getDoubleAttribute("width"),
        e->getDoubleAttribute("height"));
    targetEditors.emplace_back(input);
}

void UserInterface::LoadAudioBin(XmlElement* e, AudioBin* l)
{
    addAndMakeVisible(l);
    AudioBinWithBounds* input = new AudioBinWithBounds;
    input->Interface = l;
    input->Bounds = Rectangle<double>(e->getDoubleAttribute("left"),
        e->getDoubleAttribute("top"),
        e->getDoubleAttribute("width"),
        e->getDoubleAttribute("height"));
    audioBins.emplace_back(input);
}

void UserInterface::AddControllerListeners(GeneController* controller)
{
    for (auto& d : displays) {
        controller->AddListener(d->Display);
    }
}

void UserInterface::AddSettingsListeners(Settings* settings)
{
    for (auto& i : interfaces)
        settings->AddListener(i->Interface);
    for (auto& i : targetEditors)
        settings->AddListener(i->Interface);
    for (auto& i : audioBins)
        settings->AddListener(i->Interface);
}

void UserInterface::LinkInternals()
{
    for (auto& t : targetEditors) {
        for (auto& a : audioBins) {
            a->Interface->AddListener(t->Interface);
        }
    }
}