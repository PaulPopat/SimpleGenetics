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

Biology::ComplexDouble GetChannelLocation(int Channel, int NumChannels);

double GetChannelAmp(Biology::ComplexDouble Location, int NumChannels, int Channel);

Biology::ComplexDouble GetPosition(Array<double> Channels);
}

#endif // CHANNELCALCULATOR_H_INCLUDED
