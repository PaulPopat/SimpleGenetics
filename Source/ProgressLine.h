/*
  ==============================================================================

    ProgressLine.h
    Created: 20 Feb 2016 10:34:31am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef PROGRESSLINE_H_INCLUDED
#define PROGRESSLINE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ListenerComponents.h"
#include "CustomLookAndFeel.h"
#include "Settings.h"


class ProgressLine    : public GeneControllerListenerComponent
{
public:
    ProgressLine(Settings * s);
    ~ProgressLine();

    void paint (Graphics&) override;
    void resized() override;
    
    void BreedComplete(const BreedCompleteData & data) override;

private:
    Rectangle<int> bounds;
    Array<double> progress;
    Settings * settings;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgressLine)
};


#endif  // PROGRESSLINE_H_INCLUDED
