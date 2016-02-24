/*
  ==============================================================================

    TargetEditor.cpp
    Created: 18 Feb 2016 3:52:18pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TargetEditor.h"

TargetEditor::TargetEditor(String Title, String Name)
    : SettingsListenerComponent(Name)
{
    title = Title;
    selected = nullptr;
    setRepaintsOnMouseActivity(true);
}

TargetEditor::~TargetEditor()
{
}

void TargetEditor::paint(Graphics& g)
{
    if (!loaded) {
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::Foreground));
        g.fillRect(bounds);
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::Text));
        g.setFont(32);
        g.drawText("Settings Not Loaded", bounds, Justification::centred);
        return;
    }
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::TextLow));
    g.setFont(32);
    g.drawText(title, bounds, Justification::centred);

    TargetArray temp = data;

    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Line));
    g.setFont(9);
    for (const auto& t : temp) {
        int pos = dataToScreen(t).x;
        g.drawLine(pos, bounds.getY(), pos, bounds.getBottom(), 2);
        g.drawText(t.Name, pos + 5, bounds.getY() + 5, 50, 9, Justification::left);
    }

    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    g.drawRect(bounds, 2);
}

void TargetEditor::resized()
{
    bounds = Rectangle<int>(2, 2, getWidth() - 4, getHeight() - 4);
}

void TargetEditor::SettingsChanged(Settings* s)
{
    loaded = s->IsLoaded();
    if (!loaded)
        return;
    XmlElement e = s->GetRawData(elementName);
    data.clear();
    forEachXmlChildElement(e, c)
    {
        data.add(TargetLocation{ c->getStringAttribute("value"), c->getDoubleAttribute("loc") });
    }
    repaint();
}

void TargetEditor::GetSettings(Settings* s)
{
    XmlElement* e = new XmlElement(elementName);

    for (int i = 0; i < data.size(); i++) {
        XmlElement* s = new XmlElement("Ele" + String(1));
        s->setAttribute("value", data[i].Name);
        s->setAttribute("loc", data[i].Position);
        e->addChildElement(s);
    }

    s->AddXmlElement(e);
}

void TargetEditor::mouseDrag(const MouseEvent& event)
{
    if (!loaded)
        return;
    if (selected != nullptr) {
        *selected = screenToData(event.getPosition(), *selected);
        if (selected->Position > 1)
            selected->Position = 1;
        else if (selected->Position < 0)
            selected->Position = 0;
        repaint();
    }
}

void TargetEditor::mouseDown(const MouseEvent& event)
{
    if (!loaded)
        return;
    selected = nullptr; // if the mouse is brought up off component something may be selected
    if (event.mods.isCtrlDown()) {
        for (int i = 0; i < data.size(); i++) {
            if (isInPoint(event.getPosition(), data[i])) {
                data.remove(i);
                repaint();
            }
        }
    }
    else {
        for (int i = 0; i < data.size(); i++) {
            if (isInPoint(event.getPosition(), data[i])) {
                selected = &data.getReference(i);
            }
        }
    }
}

void TargetEditor::mouseUp(const MouseEvent& event)
{
    if (!loaded)
        return;
    selected = nullptr;
}

void TargetEditor::AddAudio(String Name)
{
    data.add(TargetLocation{ Name, 0.5 });
    repaint();
}

double TargetEditor::RangeValue(double val, double min, double max)
{
    return (val - min) / (max - min);
}

bool TargetEditor::isInPoint(const Point<int>& input, const TargetLocation& point)
{
    const Point<float> i = dataToScreen(point);
    return input.x - i.x >= -3 && input.x - i.x <= 3;
}

TargetLocation TargetEditor::screenToData(const Point<int>& input, const TargetLocation& reference)
{
    return TargetLocation{ reference.Name, RangeValue(input.x, bounds.getX(), bounds.getRight()) };
}

const Point<float> TargetEditor::dataToScreen(const TargetLocation& data)
{
    return Point<float>(bounds.getX() + (data.Position * bounds.getWidth()), 0);
}
