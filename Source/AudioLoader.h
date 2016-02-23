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
#include "fftw3.h"
#include <cmath>

namespace FFTW {

struct AudioAnalysis {
    AudioAnalysis() = default;
    AudioAnalysis(int bandSize) { Amplitude.insertMultiple(0, 0, bandSize); }
    Array<double> Amplitude;
    Biology::ComplexDouble Position;
};

class AudioLoader {
public:
    AudioLoader(int FFTSize);
    ~AudioLoader();
    AudioAnalysis AnalyzeAudio(File Path, int NumBands, int Band);

private:
    AudioSampleBuffer LoadAudio(File Path);
    Array<Array<double> > GetAmplitudes(const AudioSampleBuffer& Buf);
    fftw_complex* output;
    double* input;
    fftw_plan fft;
    int fftSize;
    ScopedPointer<AudioFormatReader> format;
};
}

#endif // AUDIOLOADER_H_INCLUDED
