/*
  ==============================================================================

    ChannelCalculator.cpp
    Created: 12 Feb 2016 4:53:06pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "ChannelCalculator.h"

Biology::ComplexDouble Utilities::GetChannelLocation(int Channel, int NumChannels)
{
    Biology::ComplexDouble output;

    double degrees = ((double)Channel / (double)NumChannels) * 360;
    double angle = (degrees * M_PI) / 180;
    output.r = cos(angle);
    output.i = sin(angle);

    return output;
}

double Utilities::GetChannelAmp(Biology::ComplexDouble Location, int NumChannels, int Channel)
{
    Array<double> output;
    double total = 0;
    for (int i = 0; i < NumChannels; i++) {
        Biology::ComplexDouble channelLoc = GetChannelLocation(i, NumChannels);
        output.add(1 - (std::sqrt(std::pow(channelLoc.r - Location.r, 2) + std::pow(channelLoc.i - Location.i, 2)) * 0.5));
        output.getReference(i) = std::pow(output[i], 6);
        total += output[i];
    }
    total = 1 / total;
    for (int i = 0; i < NumChannels; i++) {
        output.getReference(i) *= total;
    }
    return output[Channel];
}

Biology::ComplexDouble Utilities::GetPosition(Array<double> Channels)
{
    double total = 0;
    for (int i = 0; i < Channels.size(); i++) {
        total += Channels[i];
    }
    double multiplier = 1 / total;
    for (int i = 0; i < Channels.size(); i++) {
        Channels.getReference(i) *= multiplier;
    }
    Biology::ComplexDouble output{ 0, 0 };

    for (int i = 0; i < Channels.size(); i++) {
        Biology::ComplexDouble channel = GetChannelLocation(i, Channels.size());
        output.r += channel.r * Channels[i];
        output.i += channel.i * Channels[i];
    }

    return output;
}