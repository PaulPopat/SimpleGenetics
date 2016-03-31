/*
  ==============================================================================

    BandCalculator.h
    Created: 12 Feb 2016 2:13:04pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef BANDCALCULATOR_H_INCLUDED
#define BANDCALCULATOR_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"

namespace Utilities {

int GetBandSize(int NumBands, int Band, int FFTSize);

int GetBandBase(int NumBands, int Band, int FFTSize);

std::vector<double> SplitToBand(const std::vector<double>& input, int NumBands, int Band);
}

#endif // BANDCALCULATOR_H_INCLUDED
