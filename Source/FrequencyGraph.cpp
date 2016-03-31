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

void FrequencyGraph::paint(Graphics& g)
{
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    g.drawRect(2, 2, getWidth() - 4, getHeight() - 4, 2);

    const std::vector<std::vector<double> >& amp = amplitudes;

    if (DataIsPopulated(amp)) {
        Path graph = BuildPath(std::move(amp), 4, 4, getWidth() - 8, getHeight() - 8);
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::DataHigh));
        g.strokePath(graph, PathStrokeType(1));
    }

    const std::vector<std::vector<double> >& tar = targets;

    if (DataIsPopulated(tar)) {
        Path graph = BuildPath(std::move(tar), 4, 4, getWidth() - 8, getHeight() - 8);
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::TargetLow));
        g.strokePath(graph, PathStrokeType(1));
    }
}

void FrequencyGraph::resized()
{
}

void FrequencyGraph::BreedComplete(const BreedCompleteData& data)
{
    if (data.ident >= amplitudes.size())
        amplitudes.resize(data.ident + 1);
    std::vector<double>& r = amplitudes[data.ident];
    r = data.amplitude;

    if (data.ident >= targets.size())
        targets.resize(data.ident + 1);
    std::vector<double>& t = targets[data.ident];
    t = data.target;

    triggerAsyncUpdate();
}

Path FrequencyGraph::BuildPath(const std::vector<std::vector<double> >& data,
                               int xmod, int ymod, int width, int height) const
{
    std::vector<double> f;
    for (const auto& da : data)
        for (const auto& d : da)
            f.emplace_back(d);

    int gap = f.size() / width;
    if (gap == 0)
        gap = 1;
    Path output;
    output.startNewSubPath(xmod, (getHeight() / 4) * GetRange(f, 0, gap) + ymod);

    for (int i = 1; i < width; i++) {
        output.lineTo(i + xmod, (getHeight() / 4) * GetRange(f, i * gap, gap) + ymod);
    }

    return output;
}

double FrequencyGraph::GetRange(const std::vector<double>& data, int start, int gap) const
{
    double a = 0;
    for (int p = start; p < start + gap; p++) {
        if (p < data.size()) {
            a += std::log10(data[p]);
        }
    }
    a /= gap;
    a += 2;
    a = 4 - a;
    return a;
}

bool FrequencyGraph::DataIsPopulated(const std::vector<std::vector<double> >& data) const
{
    if (data.empty())
        return false;
    for (auto& a : data)
        if (a.empty())
            return false;
    return true;
}