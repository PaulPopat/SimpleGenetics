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
        data.emplace_back(f.createInputStream());
    }
    String name = OutputPath.getFileNameWithoutExtension();
    OutputPath = OutputPath.getParentDirectory();
    for (int i = 0; i < Channels; i++) {
        File loc(File(OutputPath.getFullPathName() + "/" + name + String(i + 1) + ".aif"));
        if (loc.existsAsFile())
            loc.deleteFile();
        AiffAudioFormat f;
        AudioFormatWriter* input = f.createWriterFor(loc.createOutputStream(), SampleRate, 1, BitDepth, 1, 0);
        writer.emplace_back(input);
    }

    for (int d = 0; d < data.size(); d++) {
        fftSize = data[d]->readInt();
        dataSize.emplace_back(data[d]->readInt());
    }

    channels = Channels;
    bitDepth = BitDepth;
    progress = 0;

    buffer.setSize(1, fftSize * 2);

    input = fftw_c(fftw_alloc_complex(fftSize + 1));
    output = fftw_r(fftw_alloc_real(fftSize * 2));
    ifft = SelfDestructPlan(fftw_plan_dft_c2r_1d(fftSize * 2, input.get(), output.get(), FFTW_ESTIMATE));
}

void FFTW::AudioWriter::run()
{
    while (data[0]->getPosition() < data[0]->getTotalLength()) {
        ComplexVector fftframe;
        ComplexVector panning;
        for (int d = 0; d < data.size(); d++) {
            AddFrameToComplexVector(fftframe, data[d], dataSize[d]);
            AddFrameToComplexVector(panning, data[d], dataSize[d]);
        }

        for (int c = 0; c < channels; c++) {
            ComplexVector cframe = GetPannedVector(fftframe, panning, c);
            PopulateBufferFromComplex(cframe);
            writer[c]->writeFromAudioSampleBuffer(buffer, 0, fftSize * 2);
        }
        progress = (double)data[0]->getPosition() / (double)data[0]->getTotalLength();
        if (threadShouldExit()) {
            progress = 0;
            return;
        }
    }
}

void FFTW::AudioWriter::AddFrameToComplexVector(ComplexVector& input, InputStream* stream, int streamSize)
{
    for (int i = 0; i < streamSize; i++) {
        input.emplace_back(std::complex<double>{ stream->readDouble(), stream->readDouble() });
    }
}

ComplexVector FFTW::AudioWriter::GetPannedVector(ComplexVector frame, ComplexVector panning, int Channel)
{
    ComplexVector output;
    for (int i = 0; i < frame.size(); i++) {
        float camp = Utilities::GetChannelAmp(panning[i], channels, Channel);
        std::complex<double> input{ frame[i].real() * camp, frame[i].imag() * camp };
        output.emplace_back(input);
    }
    return output;
}

void FFTW::AudioWriter::PopulateBufferFromComplex(ComplexVector frame)
{
    input.get()[fftSize][0] = 0;
    input.get()[fftSize][1] = 0;
    for (int i = 0; i < fftSize; i++) {
        input.get()[i][0] = frame[i].real();
        input.get()[i][1] = frame[i].imag();
    }
    fftw_execute(ifft);
    for (int i = 0; i < fftSize * 2; i++) {
        buffer.setSample(0, i, output.get()[i] / (fftSize / 2 + 1));
    }
}