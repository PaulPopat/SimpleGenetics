/*
  ==============================================================================

    ChannelCalculator.h
    Created: 12 Feb 2016 11:51:47am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef CHANNELCALCULATOR_H_INCLUDED
#define CHANNELCALCULATOR_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "ComplexFrame.h"

namespace Utilities {

std::complex<double> GetChannelLocation(int Channel, int NumChannels);

double GetChannelAmp(std::complex<double> Location, int NumChannels, int Channel);

std::complex<double> GetPosition(std::vector<double> Channels);
}

#endif // CHANNELCALCULATOR_H_INCLUDED
