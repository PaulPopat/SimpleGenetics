/*
  ==============================================================================

    LiveAudioDecoder.h
    Created: 19 Feb 2016 2:35:46pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef LIVEAUDIODECODER_H_INCLUDED
#define LIVEAUDIODECODER_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "Gene.h"
#include "GeneController.h"
#include "Settings.h"
#include "fftw3.h"

namespace FFTW {

class LiveAudioDecoder : public Thread, public GeneController::Listener {
public:
    LiveAudioDecoder(int Bands, int FFTSize, int FramesPerGene);
    ~LiveAudioDecoder();

    /* main thread loop */
    void run() override;
    /* called from one of the gene threads populating data to transform */
    void BreedComplete(const BreedCompleteData& data) override;
    /* called from the audio thread to get any complete data */
    const Array<double> GetCurrentAudio(int numSamples);

private:
    Array<double> currentAudio;
    Array<Biology::Gene> timbre;
    const int bands;
    const int fftSize;
    const int framesPerGene;

    int playhead;

    Array<double> audioFromFrame(const Array<std::complex<double>>& bands);
    bool vectorFull(const Array<Biology::Gene>& input);

    fftw_complex* input;
    double* output;
    fftw_plan ifft;
};
}

#endif // LIVEAUDIODECODER_H_INCLUDED
