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
#include "../Settings/CustomLookAndFeel.h"
#include "../Utilities/ListenerComponents.h"
#include <cmath>

class FrequencyGraph : public GeneControllerListenerComponent
{
public:
  FrequencyGraph();
  void paint(Graphics &) override;
  void resized() override;

  void BreedComplete(const BreedData &d, const SettingsData &s) override;

private:
  Path BuildPath(const std::vector<std::vector<double>> &data, int xmod, int ymod, int width, int height) const;
  double GetRange(const std::vector<double> &data, int start, int gap) const;
  bool DataIsPopulated(const std::vector<std::vector<double>> &data) const;
  std::vector<std::vector<double>> amplitudes;
  std::vector<std::vector<double>> targets;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencyGraph)
};

#endif // FREQUENCYGRAPH_H_INCLUDED
