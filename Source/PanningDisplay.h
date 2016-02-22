/*
  ==============================================================================

    PanningDisplay.h
    Created: 16 Feb 2016 9:54:26am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef PANNINGDISPLAY_H_INCLUDED
#define PANNINGDISPLAY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomLookAndFeel.h"
#include "ListenerComponents.h"

class PanningDisplay : public GeneControllerListenerComponent {
public:
    PanningDisplay();
    ~PanningDisplay();

    void paint(Graphics&) override;
    void resized() override;

    void BreedComplete(const BreedCompleteData& data) override;

private:
    Array<FFT::Complex> positions;
    Array<FFT::Complex> targets;
    Rectangle<int> bounds;

    Colour InterpolateColour(Colour c1, Colour c2, double progress);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanningDisplay)
};

#endif // PANNINGDISPLAY_H_INCLUDED
