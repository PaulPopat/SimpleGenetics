/*
  ==============================================================================

    RandomGen.h
    Created: 11 Feb 2016 5:26:42pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef RANDOMGEN_H_INCLUDED
#define RANDOMGEN_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

namespace Utilities
{

class Random
{
  public:
    Random()
    {
        this->rng.setSeedRandomly();
    }
    double GetDouble(double min, double max)
    {
        return this->rng.nextDouble() * (max - min) + min;
    }
    float GetFloat(float min, float max)
    {
        return this->rng.nextFloat() * (max - min) + min;
    }
    int GetInt(int min, int max)
    {
        return this->rng.nextInt(Range<int>(min, max));
    }
    int GetWeightedInt(int min, int max, double weighting)
    {
        return std::pow(this->rng.nextDouble(), weighting) * ((double)max - (double)min) + (double)min;
    }
    double GetWeightedDouble(double min, double max, double weighting)
    {
        return std::pow(this->rng.nextDouble(), weighting) * (max - min) + min;
    }
    bool GetBool()
    {
        return this->rng.nextBool();
    }

  private:
    juce::Random rng;
};
} // namespace Utilities

#endif // RANDOMGEN_H_INCLUDED
