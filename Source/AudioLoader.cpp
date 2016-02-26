/*
  ==============================================================================

    AudioLoader.cpp
    Created: 12 Feb 2016 11:06:39am
    Author:  Paul Popat

  ==============================================================================
*/

#include "AudioLoader.h"

FFTW::AudioLoader::AudioLoader(int FFTSize)
    : fftSize(FFTSize)
    , output(fftw_alloc_complex(fftSize + 1))
    , input(fftw_alloc_real(fftSize * 2))
    , fft(fftw_plan_dft_r2c_1d(fftSize * 2, input.get(), output.get(), FFTW_ESTIMATE))
{
}

FFTW::AudioAnalysis FFTW::AudioLoader::AnalyzeAudio(File Path, int NumBands, int Band)
{
    int cur = Utilities::GetBandSize(NumBands, Band, fftSize);
    int base = Utilities::GetBandBase(NumBands, Band, fftSize);

    if (Path.getFileNameWithoutExtension() == "None")
        return AudioAnalysis(cur);

    AudioAnalysis out;

    AudioSampleBuffer buf = LoadAudio(Path);
    Array<Array<double> > amp = GetAmplitudes(buf);

    out.Amplitude.insertMultiple(0, 0, cur);
    for (int s = 0; s < cur; s++) {
        for (int c = 0; c < amp.size(); c++)
            out.Amplitude.getReference(s) += amp[c][base + s] / amp.size();
    }

    Array<double> channels;
    for (int c = 0; c < amp.size(); c++)
        channels.add(0);
    for (int c = 0; c < amp.size(); c++) {
        for (int s = 0; s < cur; s++) {
            channels.getReference(c) += amp[c][base + s];
        }
        channels.getReference(c) /= amp[c].size();
    }
    out.Position = Utilities::GetPosition(channels);

    return out;
}

AudioSampleBuffer FFTW::AudioLoader::LoadAudio(File Path)
{
    AiffAudioFormat reader;
    format = reader.createReaderFor(Path.createInputStream(), true);
    AudioSampleBuffer buffer = AudioSampleBuffer(format->numChannels, format->lengthInSamples);
    format->read(&buffer, 0, format->lengthInSamples, 0, true, true);
    return buffer;
}

Array<Array<double> > FFTW::AudioLoader::GetAmplitudes(const AudioSampleBuffer& Buf)
{
    Array<Array<double> > out;
    out.resize(Buf.getNumChannels());

    const int framesInFile = Buf.getNumSamples() / (fftSize * 2);
    for (int c = 0; c < Buf.getNumChannels(); c++) {
        Array<double>& r = out.getReference(c);
        for (int s = 0; s < fftSize; s++)
            r.add(0);
        for (int f = 0; f < framesInFile; f++) {

            for (int s = 0; s < fftSize * 2; s++) {
                input.get()[s] = Buf.getSample(c, f * (fftSize * 2) + s);
            }

            fftw_execute(fft);

            for (int s = 0; s < fftSize; s++) {
                r.getReference(s) += std::sqrt(std::pow(output.get()[s][0], 2) + std::pow(output.get()[s][1], 2));
            }
        }
        for (int s = 0; s < fftSize; s++) {
            r.getReference(s) /= framesInFile;
        }
    }

    return out;
}