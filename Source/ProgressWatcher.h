/*
  ==============================================================================

    ProgressWatcher.h
    Created: 18 Feb 2016 2:40:13pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef PROGRESSWATCHER_H_INCLUDED
#define PROGRESSWATCHER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomLookAndFeel.h"
#include "ListenerComponents.h"

class ProgressWatcher : public GeneControllerListenerComponent {
public:
    ProgressWatcher(String Type);
    ~ProgressWatcher();

    void paint(Graphics&) override;
    void resized() override;

    void BreedComplete(const BreedCompleteData& data) override;

private:
    std::vector<double> progress;
    std::vector<Rectangle<int> > bounds;

    String type;

    void SetUpBounds();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProgressWatcher)
};

#endif // PROGRESSWATCHER_H_INCLUDED
