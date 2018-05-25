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
#include "../Settings/CustomLookAndFeel.h"
#include "../Utilities/ListenerComponents.h"
#include "../Settings/Settings.h"

struct TargetLocation
{
    String Name;
    double Position;

    bool operator==(const TargetLocation &rhs) const { return Position == rhs.Position; }
    bool operator<(const TargetLocation &rhs) const { return Position < rhs.Position; }
    bool operator>(const TargetLocation &rhs) const { return Position > rhs.Position; }
    bool operator<=(const TargetLocation &rhs) const { return Position <= rhs.Position; }
    bool operator>=(const TargetLocation &rhs) const { return Position >= rhs.Position; }
};

using TargetArray = SortedSet<TargetLocation>;

class TargetEditor : public SettingsListenerComponent
{
  public:
    TargetEditor(String Title, String Name);

    void paint(Graphics &) override;
    void resized() override;

    void SettingsChanged(Settings *) override;
    void GetSettings(Settings *) override;

    void mouseDrag(const MouseEvent &event) override;
    void mouseDown(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;

    void AddAudio(String Name);

  private:
    String title;
    Rectangle<int> bounds;
    bool loaded = false;
    TargetLocation *selected;

    static double RangeValue(double val, double min, double max);

    bool isInPoint(const Point<int> &input, const TargetLocation &point) const;

    TargetLocation screenToData(const Point<int> &input, const TargetLocation &reference) const;
    Point<float> dataToScreen(const TargetLocation &data) const;

    TargetArray data;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TargetEditor)
};

#endif // TARGETEDITOR_H_INCLUDED
