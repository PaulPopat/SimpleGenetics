/*
  ==============================================================================

    AudioLoader.h
    Created: 12 Feb 2016 11:06:39am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef AUDIOLOADER_H_INCLUDED
#define AUDIOLOADER_H_INCLUDED
#include <cmath>
#include "fftw3.h"
#include "ChannelCalculator.h"
#include "BandCalculator.h"
#include "../JuceLibraryCode/JuceHeader.h"

namespace FFTW {
    
struct AudioAnalysis {
    AudioAnalysis() = default;
    AudioAnalysis(int bandSize) { Amplitude.insertMultiple(0, 0, bandSize); }
    Array<double> Amplitude;
    FFT::Complex Position;
};

class AudioLoader {
public:
    AudioLoader(int FFTSize);
    ~AudioLoader();
    AudioAnalysis AnalyzeAudio(File Path, int NumBands, int Band);
private:
    AudioSampleBuffer LoadAudio(File Path);
    Array<Array<double>> GetAmplitudes(const AudioSampleBuffer & Buf);
    fftw_complex * output;
    double * input;
    fftw_plan fft;
    int fftSize;
    ScopedPointer<AudioFormatReader> format;
};
    
}



#endif  // AUDIOLOADER_H_INCLUDED
