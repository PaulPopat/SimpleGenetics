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
#include "FFTWHelper.h"
#include "Gene.h"
#include "GeneController.h"
#include "Settings.h"

namespace FFTW {

class LiveAudioDecoder : public Thread, public GeneController::Listener {
public:
    LiveAudioDecoder(int Bands, int FFTSize, int FramesPerGene);

    /* main thread loop */
    void run() override;
    /* called from one of the gene threads populating data to transform */
    void BreedComplete(const BreedCompleteData& data) override;
    /* called from the audio thread to get any complete data */
    std::vector<double> GetCurrentAudio(int numSamples);

private:
    std::vector<double> currentAudio;
    std::vector<Biology::Gene> timbre;
    const int bands;
    const int fftSize;
    const int framesPerGene;

    int playhead;

    std::vector<double> audioFromFrame(const std::vector<std::complex<double> >& bands);
    bool vectorFull(const std::vector<Biology::Gene>& input);

    fftw_c input;
    fftw_r output;
    SelfDestructPlan ifft;
};
}

#endif // LIVEAUDIODECODER_H_INCLUDED
