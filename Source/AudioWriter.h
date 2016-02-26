/*
  ==============================================================================

    FFTWWrapper.h
    Created: 11 Feb 2016 10:11:05pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef FFTWWRAPPER_H_INCLUDED
#define FFTWWRAPPER_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "ChannelCalculator.h"
#include "ComplexFrame.h"
#include "FFTWHelper.h"

using ComplexVector = Array<std::complex<double>>;

namespace FFTW {

class AudioWriter : public Thread {
public:
    AudioWriter(File DataPath, File OutputPath, int SampleRate, int Channels, int BitDepth);
    double& Progress;

private:
    void run() override;
    void AddFrameToComplexVector(ComplexVector& input, InputStream* stream, int streamSize);
    ComplexVector GetPannedVector(ComplexVector frame, ComplexVector panning, int Channel);
    void PopulateBufferFromComplex(ComplexVector frame);

    fftw_c input;
    fftw_r output;
    SelfDestructPlan ifft;
    double progress;

    Array<ScopedPointer<AudioFormatWriter> > writer;
    Array<ScopedPointer<InputStream> > data;
    Array<int> dataSize;
    AudioSampleBuffer buffer;

    int fftSize;
    int channels;
    int bitDepth;
};
}

#endif // FFTWWRAPPER_H_INCLUDED
