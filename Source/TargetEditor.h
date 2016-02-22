/*
  ==============================================================================

    TargetEditor.h
    Created: 18 Feb 2016 3:52:18pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef TARGETEDITOR_H_INCLUDED
#define TARGETEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomLookAndFeel.h"
#include "ListenerComponents.h"
#include "Settings.h"

struct TargetLocation {
    String Name;
    double Position;
};

inline bool operator==(const TargetLocation& l, const TargetLocation& r) { return l.Position == r.Position; }
inline bool operator<(const TargetLocation& l, const TargetLocation& r) { return l.Position < r.Position; }
inline bool operator>(const TargetLocation& l, const TargetLocation& r) { return l.Position > r.Position; }
inline bool operator<=(const TargetLocation& l, const TargetLocation& r) { return l.Position <= r.Position; }
inline bool operator>=(const TargetLocation& l, const TargetLocation& r) { return l.Position >= r.Position; }

using TargetArray = SortedSet<TargetLocation>;

class TargetEditor : public SettingsListenerComponent {
public:
    TargetEditor(String Title, String Name);
    ~TargetEditor();

    void paint(Graphics&) override;
    void resized() override;

    void SettingsChanged(Settings*) override;
    void GetSettings(Settings*) override;

    void mouseDrag(const MouseEvent& event) override;
    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    void AddAudio(String Name);

private:
    String title;
    Rectangle<int> bounds;
    bool loaded = false;
    TargetLocation* selected;

    double RangeValue(double val, double min, double max);

    bool isInPoint(const Point<int>& input, const TargetLocation& point);

    TargetLocation screenToData(const Point<int>& input, const TargetLocation& reference);
    const Point<float> dataToScreen(const TargetLocation& data);

    TargetArray data;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TargetEditor)
};

#endif // TARGETEDITOR_H_INCLUDED
