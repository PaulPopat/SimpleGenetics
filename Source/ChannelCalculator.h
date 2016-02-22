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

namespace Utilities {

FFT::Complex GetChannelLocation(int Channel, int NumChannels);

double GetChannelAmp(FFT::Complex Location, int NumChannels, int Channel);

FFT::Complex GetPosition(Array<double> Channels);
}

#endif // CHANNELCALCULATOR_H_INCLUDED
