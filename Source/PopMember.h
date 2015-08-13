/*
  ==============================================================================

    PopMember.h
    Created: 5 Aug 2015 10:41:00am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef POPMEMBER_H_INCLUDED
#define POPMEMBER_H_INCLUDED
#include <utility>
#include <vector>
#include "fftw3.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "FFTBin.h"
#include "SettingsUtils.h"
#include "Settings.h"

class PopMember {
public:
    PopMember();
    PopMember(PopMember *Mother, PopMember *Father);
    PopMember(PopMember *Copy);
    PopMember(int BinLength, int NumBins, int NumChannels);
    ~PopMember();
    void AddMember(PopMember *Addition);
    double GetCloseness(std::vector<double> Target, Settings *settings);
    void Mutate(double Amount, Settings *settings);
    std::vector<std::vector<double> > GetAudio(fftw_complex* input, double *output, fftw_plan &ifft);
    int GetNumBins();
private:
    std::vector<std::vector<FFTBin*> > data;
    int binPosition = 0;
};


#endif  // POPMEMBER_H_INCLUDED
