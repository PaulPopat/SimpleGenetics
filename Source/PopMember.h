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
#include "../JuceLibraryCode/JuceHeader.h"
#include "FFTBin.h"
#include "SettingsUtils.h"

class PopMember {
public:
    PopMember();
    PopMember(PopMember *Mother, PopMember *Father);
    PopMember(int BinLength, int NumBins, int NumChannels);
    ~PopMember();
    void AddMember(PopMember *Addition);
    double GetCloseness(std::vector<double> Target);
    void Mutate(double Amount);
    std::vector<std::vector<double> > GetAudio();
    int GetNumBins();
private:
    std::vector<std::vector<FFTBin*> > data;
    int binPosition = 0;
};


#endif  // POPMEMBER_H_INCLUDED
