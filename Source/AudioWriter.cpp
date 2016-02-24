/*
  ==============================================================================

    FFTWWrapper.cpp
    Created: 11 Feb 2016 10:11:05pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "AudioWriter.h"

FFTW::AudioWriter::AudioWriter(File DataPath, File OutputPath, int SampleRate, int Channels, int BitDepth)
    : Thread("Audio Writer")
    , Progress(progress)
{
    DirectoryIterator dir(DataPath, true, "*.bin");
    while (dir.next()) {
        File f(dir.getFile());
        data.add(f.createInputStream());
    }
    String name = OutputPath.getFileNameWithoutExtension();
    OutputPath = OutputPath.getParentDirectory();
    for (int i = 0; i < Channels; i++) {
        File loc(File(OutputPath.getFullPathName() + "/" + name + String(i + 1) + ".aif"));
        if (loc.existsAsFile())
            loc.deleteFile();
        AiffAudioFormat f;
        AudioFormatWriter* input = f.createWriterFor(loc.createOutputStream(), SampleRate, 1, BitDepth, 1, 0);
        writer.add(input);
    }

    for (int d = 0; d < data.size(); d++) {
        fftSize = data.getReference(d)->readInt();
        dataSize.add(data.getReference(d)->readInt());
    }

    channels = Channels;
    bitDepth = BitDepth;
    progress = 0;

    buffer.setSize(1, fftSize * 2);
    input = fftw_alloc_complex(fftSize + 1);
    output = fftw_alloc_real(fftSize * 2);
    ifft = fftw_plan_dft_c2r_1d(fftSize * 2, input, output, FFTW_ESTIMATE);
}

FFTW::AudioWriter::~AudioWriter()
{
    fftw_destroy_plan(ifft);
    fftw_free(input);
    fftw_free(output);
}

void FFTW::AudioWriter::run()
{
    while (data.getReference(0)->getPosition() < data.getReference(0)->getTotalLength()) {
        ComplexVector fftframe;
        ComplexVector panning;
        for (int d = 0; d < data.size(); d++) {
            AddFrameToComplexVector(fftframe, data.getReference(d), dataSize.getReference(d));
            AddFrameToComplexVector(panning, data.getReference(d), dataSize.getReference(d));
        }

        for (int c = 0; c < channels; c++) {
            ComplexVector cframe = GetPannedVector(fftframe, panning, c);
            PopulateBufferFromComplex(cframe);
            writer.getReference(c)->writeFromAudioSampleBuffer(buffer, 0, fftSize * 2);
        }
        progress = (double)data.getReference(0)->getPosition() / (double)data.getReference(0)->getTotalLength();
        if (threadShouldExit()) {
            progress = 0;
            return;
        }
    }
}

void FFTW::AudioWriter::AddFrameToComplexVector(ComplexVector& input, InputStream* stream, int streamSize)
{
    for (int i = 0; i < streamSize; i++) {
        input.add(std::complex<double>{ stream->readDouble(), stream->readDouble() });
    }
}

ComplexVector FFTW::AudioWriter::GetPannedVector(ComplexVector frame, ComplexVector panning, int Channel)
{
    ComplexVector output;
    for (int i = 0; i < frame.size(); i++) {
        float camp = Utilities::GetChannelAmp(panning[i], channels, Channel);
        std::complex<double> input{ frame[i].real() * camp, frame[i].imag() * camp };
        output.add(input);
    }
    return output;
}

void FFTW::AudioWriter::PopulateBufferFromComplex(ComplexVector frame)
{
    input[fftSize][0] = 0;
    input[fftSize][1] = 0;
    for (int i = 0; i < fftSize; i++) {
        input[i][0] = frame[i].real();
        input[i][1] = frame[i].imag();
    }
    fftw_execute(ifft);
    for (int i = 0; i < fftSize * 2; i++) {
        buffer.setSample(0, i, output[i] / (fftSize / 2 + 1));
    }
}