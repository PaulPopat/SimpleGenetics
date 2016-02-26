/*
  ==============================================================================

    ChannelCalculator.cpp
    Created: 12 Feb 2016 4:53:06pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "ChannelCalculator.h"

std::complex<double> Utilities::GetChannelLocation(int Channel, int NumChannels)
{

    double degrees = ((double)Channel / (double)NumChannels) * 360;
    double angle = (degrees * M_PI) / 180;

    return std::complex<double>{ cos(angle), sin(angle) };
}

double Utilities::GetChannelAmp(std::complex<double> Location, int NumChannels, int Channel)
{
    Array<double> output;
    double total = 0;
    for (int i = 0; i < NumChannels; i++) {
        std::complex<double> channelLoc = GetChannelLocation(i, NumChannels);
        output.add(1 - (std::sqrt(std::pow(channelLoc.real() - Location.real(), 2) + std::pow(channelLoc.imag() - Location.imag(), 2)) * 0.5));
        output.getReference(i) = std::pow(output[i], 6);
        total += output[i];
    }
    total = 1 / total;
    for (int i = 0; i < NumChannels; i++) {
        output.getReference(i) *= total;
    }
    return output[Channel];
}

std::complex<double> Utilities::GetPosition(Array<double> Channels)
{
    double total = 0;
    for (int i = 0; i < Channels.size(); i++) {
        total += Channels[i];
    }
    double multiplier = 1 / total;
    for (int i = 0; i < Channels.size(); i++) {
        Channels.getReference(i) *= multiplier;
    }
    std::complex<double> output{ 0, 0 };

    for (int i = 0; i < Channels.size(); i++) {
        std::complex<double> channel = GetChannelLocation(i, Channels.size());
        output += channel * Channels[i];
    }

    return output;
}