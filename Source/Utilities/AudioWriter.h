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
#include "../Algorithm/ComplexFrame.h"
#include "FFTWHelper.h"

using ComplexVector = std::vector<std::complex<double>>;

namespace FFTW
{

class AudioWriter : public Thread
{
public:
  AudioWriter(File DataPath, File OutputPath, int SampleRate, int Channels, int BitDepth);
  ~AudioWriter() { fftw_destroy_plan(ifft); }
  double &Progress;

private:
  void run() override;
  void AddFrameToComplexVector(ComplexVector &input, InputStream *stream, int streamSize);
  ComplexVector GetPannedVector(ComplexVector frame, ComplexVector panning, int Channel);
  void PopulateBufferFromComplex(ComplexVector frame);

  fftw_c input;
  fftw_r output;
  fftw_plan ifft;
  double progress;

  std::vector<ScopedPointer<AudioFormatWriter>> writer;
  std::vector<ScopedPointer<InputStream>> data;
  std::vector<int> dataSize;
  AudioSampleBuffer buffer;

  int fftSize;
  int channels;
  int bitDepth;
};
}

#endif // FFTWWRAPPER_H_INCLUDED
