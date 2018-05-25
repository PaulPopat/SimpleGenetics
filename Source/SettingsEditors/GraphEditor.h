/*
  ==============================================================================

    GraphEditor.h
    Created: 18 Feb 2016 3:51:17pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef GRAPHEDITOR_H_INCLUDED
#define GRAPHEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Settings/CustomLookAndFeel.h"
#include "../Algorithm/GeneController.h"
#include "../Utilities/ListenerComponents.h"
#include "../Settings/Settings.h"
#include <limits>

struct GraphPoint
{
    double Value;
    double Position;

    bool operator==(const GraphPoint &rhs) const { return Position == rhs.Position; }
    bool operator<(const GraphPoint &rhs) const { return Position < rhs.Position; }
    bool operator>(const GraphPoint &rhs) const { return Position > rhs.Position; }
    bool operator<=(const GraphPoint &rhs) const { return Position <= rhs.Position; }
    bool operator>=(const GraphPoint &rhs) const { return Position >= rhs.Position; }
};

class GraphEditor : public SettingsListenerComponent
{
  public:
    GraphEditor(String Title, String Name);

    void paint(Graphics &) override;
    void resized() override;

    void SettingsChanged(Settings *) override;
    void GetSettings(Settings *) override;

    void mouseDrag(const MouseEvent &event) override;
    void mouseDown(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;

  protected:
    virtual void floorData();
    SortedSet<GraphPoint> data;

  private:
    String title;
    Rectangle<int> bounds;
    bool loaded = false;
    double max;
    double min;
    GraphPoint *selected;
    Settings *settings;

    static double RangeValue(double val, double min, double max);

    bool isInPoint(const Point<int> &input, const GraphPoint &point) const;

    GraphPoint screenToData(const Point<int> &input) const;
    Point<float> dataToScreen(const GraphPoint &data) const;
    void setMinMax();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphEditor)
};

#endif // GRAPHEDITOR_H_INCLUDED
