/*
  ==============================================================================

    FrequencyGraph.cpp
    Created: 14 Feb 2016 12:25:44pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequencyGraph.h"

FrequencyGraph::FrequencyGraph() {
    setInterceptsMouseClicks(false, false);
}

void FrequencyGraph::paint (Graphics& g)
{
    g.setColour(findColour(CustomLookAndFeel::ColourIDs::Outline));
    g.drawRect(2, 2, getWidth() - 4, getHeight() - 4, 2);
    
    const Array<Array<double>> & amp = amplitudes;
    
    if (DataIsPopulated(amp)) {
        Path graph = BuildPath(std::move(amp), 4, 4, getWidth() - 8, getHeight() - 8);
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::DataHigh));
        g.strokePath(graph, PathStrokeType(1));
    }
    
    const Array<Array<double>> & tar = targets;
    
    if (DataIsPopulated(tar)) {
        Path graph = BuildPath(std::move(tar), 4, 4, getWidth() - 8, getHeight() - 8);
        g.setColour(findColour(CustomLookAndFeel::ColourIDs::TargetLow));
        g.strokePath(graph, PathStrokeType(1));
    }
    
}

void FrequencyGraph::resized()
{
}


void FrequencyGraph::BreedComplete(const BreedCompleteData & data) {
    if (data.ident >= amplitudes.size())
        amplitudes.resize(data.ident + 1);
    Array<double> & r = amplitudes.getReference(data.ident);
    if (r.size() > 0)
        r.clear();
    
    for (int i = 0 ; i < data.amplitude.size() ; i++)
        r.add(data.amplitude[i]);
    
    if (data.ident >= targets.size())
        targets.resize(data.ident + 1);
    Array<double> & t = targets.getReference(data.ident);
    if (t.size() > 0)
        t.clear();
    
    for (int i = 0 ; i < data.target.size() ; i++)
        t.add(data.target[i]);
    
    triggerAsyncUpdate();
}

Path FrequencyGraph::BuildPath(const Array<Array<double>> && data, int xmod, int ymod, int width, int height) {
    Array<double> f;
    for (const auto & da : data) for (const auto & d : da) f.add(d);
    
    int gap = f.size() / width;
    if (gap == 0) gap = 1;
    Path output;
    output.startNewSubPath(xmod, (getHeight() / 4) * GetRange(f, 0, gap) + ymod);
    
    for (int i = 1 ; i < width ; i++) {
        output.lineTo(i + xmod, (getHeight() / 4) * GetRange(f, i * gap, gap) + ymod);
    }
    
    return output;
}

double FrequencyGraph::GetRange(const Array<double> &data, int start, int gap) {
    double a = 0;
    for (int p = start ; p < start + gap ; p++) {
        if (p < data.size()) {
            a += std::log10(data[p]);
        }
    }
    a /= gap;
    a += 2;
    a = 4 - a;
    return a;
}

bool FrequencyGraph::DataIsPopulated(const Array<Array<double>> & data) {
    if (data.size() == 0) return false;
    else for(auto & a : data) if (a.size() == 0) return false;
    return true;
}