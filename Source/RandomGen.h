/*
  ==============================================================================

    RandomGen.h
    Created: 11 Feb 2016 5:26:42pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef RANDOMGEN_H_INCLUDED
#define RANDOMGEN_H_INCLUDED
#include <cmath>
#include "../JuceLibraryCode/JuceHeader.h"

namespace Utilities {

class Random {
public:
    Random() {
        rng.setSeedRandomly();
    }
    double GetDouble(double min, double max) {
        return rng.nextDouble() * (max - min) + min;
    }
    float GetFloat(float min, float max) {
        return rng.nextFloat() * (max - min) + min;
    }
    int GetInt(int min, int max) {
        return rng.nextInt(Range<int>(min, max));
    }
    int GetWeightedInt(int min, int max, double weighting) {
        return std::pow(rng.nextDouble(), weighting) * ((double)max - (double)min) + (double)min;
    }
    double GetWeightedDouble(double min, double max, double weighting) {
        return std::pow(rng.nextDouble(), weighting) * (max - min) + min;
    }
    bool GetBool() {
        return rng.nextBool();
    }
private:
    juce::Random rng;
};
    
}


#endif  // RANDOMGEN_H_INCLUDED
