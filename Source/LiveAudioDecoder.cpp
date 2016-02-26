/*
  ==============================================================================

    LiveAudioDecoder.cpp
    Created: 19 Feb 2016 2:35:46pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "LiveAudioDecoder.h"

FFTW::LiveAudioDecoder::LiveAudioDecoder(int Bands, int FFTSize, int FramesPerGene)
    : Thread("AudioDecoder")
    , bands(Bands)
    , fftSize(FFTSize)
    , framesPerGene(FramesPerGene)
    , playhead(0)
    , input(fftw_alloc_complex(fftSize + 1))
    , output(fftw_alloc_real(fftSize * 2))
    , ifft(fftw_plan_dft_c2r_1d(fftSize * 2, input.get(), output.get(), FFTW_ESTIMATE))
{
    for (int i = 0; i < Bands; i++) {
        timbre.add(Biology::Gene());
    }
}

void FFTW::LiveAudioDecoder::run()
{
    while (true) {
        Array<Biology::Gene> temp = timbre;

        if (vectorFull(temp)) {

            Array<double> audioTemp;

            for (int i = 0; i < framesPerGene; i++) {
                Array<std::complex<double> > frame;
                for (int j = 0; j < temp.size(); j++) {
                    int s = temp[j].GetFrame(i).GetData().size();
                    for (int b = 0; b < s; b++)
                        frame.add(temp[j].GetFrame(i).GetData().getUnchecked(b));
                }

                Array<double> frameaudio = audioFromFrame(frame);
                for (int f = 0; f < frameaudio.size(); f++)
                    audioTemp.add(frameaudio[f]);
            }

            currentAudio = audioTemp;
        }
        if (threadShouldExit())
            return;
    }
}

void FFTW::LiveAudioDecoder::BreedComplete(const BreedCompleteData& data)
{
    timbre.getReference(data.ident) = data.timbreData;
}

Array<double> FFTW::LiveAudioDecoder::GetCurrentAudio(int numSamples)
{
    Array<double> returndata;

    if (currentAudio.size() > 0) {
        for (int i = 0; i < numSamples; i++) {
            returndata.add(currentAudio[(i + playhead) % currentAudio.size()]);
        }
        playhead += numSamples;
        playhead %= currentAudio.size();
    }

    return returndata;
}

Array<double> FFTW::LiveAudioDecoder::audioFromFrame(const Array<std::complex<double> >& frame)
{
    input.get()[fftSize][0] = 0;
    input.get()[fftSize][1] = 0;
    for (int i = 0; i < fftSize; i++) {
        input.get()[i][0] = frame[i].real();
        input.get()[i][1] = frame[i].imag();
    }
    fftw_execute(ifft);
    Array<double> final;
    for (int i = 0; i < fftSize * 2; i++) {
        final.add(output.get()[i] / (fftSize / 2 + 1));
    }
    return final;
}

bool FFTW::LiveAudioDecoder::vectorFull(const Array<Biology::Gene>& input)
{
    if (input.size() != bands)
        return false;
    for (int i = 0; i < input.size(); i++) {
        if (input.getReference(i).GetNumFrames() != framesPerGene)
            return false;
    }
    return true;
}