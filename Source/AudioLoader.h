/*
  ==============================================================================

    AudioLoader.h
    Created: 12 Feb 2016 11:06:39am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef AUDIOLOADER_H_INCLUDED
#define AUDIOLOADER_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "BandCalculator.h"
#include "ChannelCalculator.h"
#include "ComplexFrame.h"
#include "FFTWHelper.h"
#include <cmath>

namespace FFTW {

struct AudioAnalysis {
    AudioAnalysis() = default;
    AudioAnalysis(int bandSize) { Amplitude.resize(bandSize, 0); }
    std::vector<double> Amplitude;
    std::complex<double> Position;
};

class AudioLoader {
public:
    AudioLoader(int FFTSize);
    AudioAnalysis AnalyzeAudio(File Path, int NumBands, int Band);

private:
    AudioSampleBuffer LoadAudio(File Path);
    std::vector<std::vector<double> > GetAmplitudes(const AudioSampleBuffer& Buf);
    int fftSize;
    fftw_c output;
    fftw_r input;
    SelfDestructPlan fft;
    ScopedPointer<AudioFormatReader> format;
};
}

#endif // AUDIOLOADER_H_INCLUDED
