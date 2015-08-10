/*
  ==============================================================================

    FFTBin.h
    Created: 5 Aug 2015 10:43:12am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef FFTBIN_H_INCLUDED
#define FFTBIN_H_INCLUDED
#include <utility>
#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"
double RandomVal(double min, double max, double weighting);

class FFTBin {
public:
    FFTBin(int size);
    FFTBin(std::vector<FFTBin*> others);
    FFTBin(FFTBin* other);
    ~FFTBin();
    std::vector<double> GetMagnitudes();
    void Mutate(double amount);
    std::vector<std::pair<double, double> > GetData();
    int GetBinSize();
private:
    std::vector<std::pair<double, double> > data;
};

#endif  // FFTBIN_H_INCLUDED
