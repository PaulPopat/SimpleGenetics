/*
  ==============================================================================

    GraphEditor.cpp
    Created: 18 Feb 2016 3:51:17pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GraphEditor.h"

GraphEditor::GraphEditor(String Title, String Name)
    : SettingsListenerComponent(Name)
{
    title = Title;
    max = 0;
    min = std::numeric_limits<float>::max();
    selected = nullptr;
    setRepaintsOnMouseActivity(true);
}

void GraphEditor::paint(Graphics& g)
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
    g.setFont(9);
    g.drawText(String(max), 4, 4, 50, 9, Justification::left);
    g.drawText(String(min), 4, getHeight() - 13, 50, 10, Justification::left);
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Text));

    SortedSet<GraphPoint> temp = data;

    if (selected != nullptr)
        *selected = screenToData(getMouseXYRelative());

    Path line;
    line.startNewSubPath(bounds.getX(), dataToScreen(temp[0]).getY());
    for (const auto& t : temp) {
        const Point<float> p = dataToScreen(t);
        line.lineTo(p);
        g.fillRect((int)p.x - 3, (int)p.y - 3, 6, 6);
    }
    line.lineTo(bounds.getRight(), line.getCurrentPosition().getY());

    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Line));
    g.strokePath(line, PathStrokeType(1));

    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    g.drawRect(bounds, 2);
}

void GraphEditor::resized()
{
    bounds = Rectangle<int>(2, 2, getWidth() - 4, getHeight() - 4);
}

void GraphEditor::SettingsChanged(Settings* s)
{
    loaded = s->IsLoaded();
    if (!loaded)
        return;
    XmlElement e = s->GetRawData(elementName);
    data.clear();
    forEachXmlChildElement(e, c)
    {
        data.add(GraphPoint{ (float)c->getDoubleAttribute("value"), (float)c->getDoubleAttribute("loc") });
    }
    floorData();
    setMinMax();
    settings = s;
    repaint();
}

void GraphEditor::GetSettings(Settings* s)
{
    XmlElement* e = new XmlElement(elementName);

    for (int i = 0; i < data.size(); i++) {
        XmlElement* s = new XmlElement("Ele" + String(1));
        s->setAttribute("value", data[i].Value);
        s->setAttribute("loc", data[i].Position);
        e->addChildElement(s);
    }

    s->AddXmlElement(e);
}

void GraphEditor::mouseDrag(const MouseEvent& event)
{
    if (!loaded)
        return;
    if (selected != nullptr) {
        *selected = screenToData(event.getPosition());
        floorData();
        setMinMax();
        repaint();
    }
}

void GraphEditor::mouseDown(const MouseEvent& event)
{
    if (!loaded)
        return;
    if (event.mods.isAltDown()) {
        data.add(screenToData(event.getPosition()));
        selected = &data.getReference(data.size() - 1);
    }
    else if (event.mods.isCtrlDown()) {
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

void GraphEditor::mouseUp(const MouseEvent& event)
{
    if (!loaded)
        return;
    selected = nullptr;
}
double GraphEditor::RangeValue(double val, double min, double max)
{
    return (val - min) / (max - min);
}

bool GraphEditor::isInPoint(const Point<int>& input, const GraphPoint& point) const
{
    const Point<float> i = dataToScreen(point);
    return input.x - i.x >= -3 && input.x - i.x <= 3 && input.y - i.y >= -3 && input.y - i.y <= 3;
}

GraphPoint GraphEditor::screenToData(const Point<int>& input) const
{
    return GraphPoint{ (float)((1 - RangeValue(input.y, bounds.getY(), bounds.getBottom())) * (max - min) + min),
        (float)RangeValue(input.x, bounds.getX(), bounds.getRight()) };
}

Point<float> GraphEditor::dataToScreen(const GraphPoint& data) const
{
    return Point<float>(bounds.getX() + (data.Position * bounds.getWidth()),
        bounds.getY() + (bounds.getHeight() - RangeValue(data.Value, min, max) * bounds.getHeight()));
}

void GraphEditor::setMinMax()
{
    max = 0;
    min = std::numeric_limits<float>::max();
    for (const auto& d : data) {
        if (d.Value * 1.2 > max)
            max = d.Value * 1.2;
        if (d.Value * 0.8 < min)
            min = d.Value * 0.8;
    }
}

void GraphEditor::floorData()
{
    for (auto& d : data) {
        if (d.Value < 0)
            d.Value = 0;
    }
}