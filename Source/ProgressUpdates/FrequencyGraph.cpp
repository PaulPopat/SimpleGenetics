/*
  ==============================================================================

    FrequencyGraph.cpp
    Created: 14 Feb 2016 12:25:44pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequencyGraph.h"

FrequencyGraph::FrequencyGraph()
{
    setInterceptsMouseClicks(false, false);
}

void FrequencyGraph::paint(Graphics &g)
{
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    g.drawRect(2, 2, getWidth() - 4, getHeight() - 4, 2);

    if (DataIsPopulated(this->amplitudes))
    {
        Path graph = BuildPath(std::move(this->amplitudes), 4, 4, getWidth() - 8, getHeight() - 8);
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::DataHigh));
        g.strokePath(graph, PathStrokeType(1));
    }

    if (DataIsPopulated(this->targets))
    {
        Path graph = BuildPath(std::move(this->targets), 4, 4, getWidth() - 8, getHeight() - 8);
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::TargetLow));
        g.strokePath(graph, PathStrokeType(1));
    }
}

void FrequencyGraph::resized()
{
}

void FrequencyGraph::BreedComplete(const BreedData &d, const SettingsData &s)
{
    this->amplitudes = d.Metrics[0].Timbre.GetSpectrum();
    this->targets = d.CurrentTarget.Amplitude;

    triggerAsyncUpdate();
}

Path FrequencyGraph::BuildPath(const std::vector<double> &data,
                               int xmod, int ymod, int width, int height) const
{
    int gap = data.size() / width;
    if (gap == 0)
        gap = 1;
    Path output;
    output.startNewSubPath(xmod, (getHeight() / 4) * GetRange(data, 0, gap) + ymod);

    for (int i = 1; i < width; i++)
    {
        output.lineTo(i + xmod, (getHeight() / 4) * GetRange(data, i * gap, gap) + ymod);
    }

    return output;
}

double FrequencyGraph::GetRange(const std::vector<double> &data, int start, int gap) const
{
    double a = 0;
    for (int p = start; p < start + gap; p++)
    {
        if (p < data.size())
        {
            a += std::log10(data[p]);
        }
    }
    a /= gap;
    a += 2;
    a = 4 - a;
    return a;
}

bool FrequencyGraph::DataIsPopulated(const std::vector<double> &data) const
{
    return !data.empty();
}