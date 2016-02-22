/*
  ==============================================================================

    ChannelCalculator.cpp
    Created: 12 Feb 2016 4:53:06pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "ChannelCalculator.h"

FFT::Complex Utilities::GetChannelLocation(int Channel, int NumChannels)
{
    FFT::Complex output;

    double degrees = ((double)Channel / (double)NumChannels) * 360;
    double angle = (degrees * M_PI) / 180;
    output.r = cos(angle);
    output.i = sin(angle);

    return output;
}

double Utilities::GetChannelAmp(FFT::Complex Location, int NumChannels, int Channel)
{
    std::vector<double> output(NumChannels, 0);
    double total = 0;
    for (int i = 0; i < NumChannels; i++) {
        FFT::Complex channelLoc = GetChannelLocation(i, NumChannels);
        output[i] = 1 - (std::sqrt(std::pow(channelLoc.r - Location.r, 2) + std::pow(channelLoc.i - Location.i, 2)) * 0.5);
        output[i] = std::pow(output[i], 6);
        total += output[i];
    }
    total = 1 / total;
    for (int i = 0; i < NumChannels; i++) {
        output[i] *= total;
    }
    return output[Channel];
}

FFT::Complex Utilities::GetPosition(Array<double> Channels)
{
    double total = 0;
    for (int i = 0; i < Channels.size(); i++) {
        total += Channels[i];
    }
    double multiplier = 1 / total;
    for (int i = 0; i < Channels.size(); i++) {
        Channels.getReference(i) *= multiplier;
    }
    FFT::Complex output{ 0, 0 };

    for (int i = 0; i < Channels.size(); i++) {
        FFT::Complex channel = GetChannelLocation(i, Channels.size());
        output.r += channel.r * Channels[i];
        output.i += channel.i * Channels[i];
    }

    return output;
}