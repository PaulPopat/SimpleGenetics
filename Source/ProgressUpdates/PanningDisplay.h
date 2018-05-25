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
#include "../Algorithm/ComplexFrame.h"
#include "../Settings/CustomLookAndFeel.h"
#include "../Utilities/ListenerComponents.h"

class PanningDisplay : public GeneControllerListenerComponent
{
public:
  PanningDisplay();

  void paint(Graphics &) override;
  void resized() override;

  void BreedComplete(const BreedData &d, const SettingsData &s) override;

private:
  std::vector<std::complex<double>> positions;
  std::vector<std::complex<double>> targets;
  Rectangle<int> bounds;

  static Colour InterpolateColour(Colour c1, Colour c2, double progress);
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanningDisplay)
};

#endif // PANNINGDISPLAY_H_INCLUDED
