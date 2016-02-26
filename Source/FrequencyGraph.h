/*
  ==============================================================================

    FrequencyGraph.h
    Created: 14 Feb 2016 12:25:44pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef FREQUENCYGRAPH_H_INCLUDED
#define FREQUENCYGRAPH_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomLookAndFeel.h"
#include "ListenerComponents.h"
#include <cmath>

class FrequencyGraph : public GeneControllerListenerComponent {
public:
    FrequencyGraph();
    void paint(Graphics&) override;
    void resized() override;

    void BreedComplete(const BreedCompleteData& data) override;

private:
    Path BuildPath(const Array<Array<double> >& data, int xmod, int ymod, int width, int height) const;
    double GetRange(const Array<double>& data, int start, int gap) const;
    bool DataIsPopulated(const Array<Array<double> >& data) const;
    Array<Array<double> > amplitudes;
    Array<Array<double> > targets;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencyGraph)
};

#endif // FREQUENCYGRAPH_H_INCLUDED
