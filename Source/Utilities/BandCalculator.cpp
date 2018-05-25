/*
  ==============================================================================

    BandCalculator.cpp
    Created: 12 Feb 2016 4:52:52pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "BandCalculator.h"

int Utilities::GetBandSize(int NumBands, int Band, int FFTSize)
{
    int splitSize = FFTSize / NumBands;
    int remainder = FFTSize % NumBands;
    int cur = splitSize;
    if (Band == NumBands - 1)
        cur += remainder;
    return cur;
}

int Utilities::GetBandBase(int NumBands, int Band, int FFTSize)
{
    int splitSize = FFTSize / NumBands;
    return splitSize * Band;
}

std::vector<double> Utilities::SplitToBand(const std::vector<double>& input, int NumBands, int Band)
{
    int bandSize = GetBandSize(NumBands, Band, input.size());
    int bandBase = GetBandBase(NumBands, Band, input.size());
    std::vector<double> output;
    for (int i = 0; i < bandSize; i++) {
        output.emplace_back(input[bandBase + i]);
    }
    return output;
}