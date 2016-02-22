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
#include "CustomLookAndFeel.h"
#include "GeneController.h"
#include "ListenerComponents.h"
#include "Settings.h"
#include <limits>

struct GraphPoint {
    double Value;
    double Position;
};

inline bool operator==(const GraphPoint& l, const GraphPoint& r) { return l.Position == r.Position; }
inline bool operator<(const GraphPoint& l, const GraphPoint& r) { return l.Position < r.Position; }
inline bool operator>(const GraphPoint& l, const GraphPoint& r) { return l.Position > r.Position; }
inline bool operator<=(const GraphPoint& l, const GraphPoint& r) { return l.Position <= r.Position; }
inline bool operator>=(const GraphPoint& l, const GraphPoint& r) { return l.Position >= r.Position; }

class GraphEditor : public SettingsListenerComponent {
public:
    GraphEditor(String Title, String Name);
    ~GraphEditor() = default;

    void paint(Graphics&) override;
    void resized() override;

    void SettingsChanged(Settings*) override;
    void GetSettings(Settings*) override;

    void mouseDrag(const MouseEvent& event) override;
    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

protected:
    virtual void floorData();
    SortedSet<GraphPoint> data;

private:
    String title;
    Rectangle<int> bounds;
    bool loaded = false;
    double max;
    double min;
    GraphPoint* selected;
    Settings* settings;

    double RangeValue(double val, double min, double max);

    bool isInPoint(const Point<int>& input, const GraphPoint& point);

    GraphPoint screenToData(const Point<int>& input);
    const Point<float> dataToScreen(const GraphPoint& data);
    void setMinMax();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphEditor)
};

#endif // GRAPHEDITOR_H_INCLUDED
