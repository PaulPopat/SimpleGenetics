/*
  ==============================================================================

    GeneController.cpp
    Created: 5 Aug 2015 12:00:06pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "GeneController.h"

GeneController::GeneController(const Settings& Settings, File Path, int Band, Utilities::Random* Gen)
    : Thread("Gene Thread " + String(Band))
{
    fftSize = Settings.GetIntValue("FFTSize");
    framesPerGene = Settings.GetIntValue("FramesPerGene");
    breedingLoops = Settings.GetIntValue("BreedingLoops");
    calculationLoops = Settings.GetIntValue("CalculationLoops");
    breedingFactor = Settings.GetIntValue("BreedingFactor");

    bandSize = Utilities::GetBandSize(Settings.GetIntValue("FrequencyBands"), Band, fftSize);

    captureInterval = Settings.GetGraph("CaptureInterval", breedingLoops);
    mutationAmount = Settings.GetGraph("MutationAmount", breedingLoops);
    mutationNumber = Settings.GetGraph("MutationNumber", breedingLoops);
    population = Settings.GetGraph("Population", breedingLoops);
    panningMutation = Settings.GetGraph("PanningMutation", breedingLoops);
    targetIdent = Settings.GetAudioGraph("Target", breedingLoops);
    target = Settings.GetAudioData("Target", fftSize, Settings.GetIntValue("FrequencyBands"), Band);

    frequencyWeighting = Utilities::SplitToBand(Settings.GetGraph("FrequencyWeighting", fftSize),
        Settings.GetIntValue("FrequencyBands"), Band);

    if (Path.existsAsFile())
        Path.deleteFile();
    Path.create();
    data = Path.createOutputStream();
    data->writeInt(fftSize);
    data->writeInt(bandSize);

    ident = Band;
    gen = Gen;
}

void GeneController::AddListener(Listener* l) { listeners.add(l); }

void GeneController::run()
{
    std::vector<Biology::Gene> timbre = InitializePopulation(population[0], true);
    std::vector<Biology::Gene> paning = InitializePopulation(population[0], false);

    // setting up memory space for this loop
    FFTW::AudioAnalysis& currenttarget = target[0]; // for putting the current target in
    double cpos = 0; // capture interval is added to this and when it reaches 1 a snapshot is taken

    for (int loop = 0; loop < calculationLoops; loop++) {
        for (int breed = 0; breed < breedingLoops; breed++) {

            cpos += 1 / captureInterval[breed];
            currenttarget = target[targetIdent[breed]];

            SortedSet<Metric> timbreMetric = GetSortedMetric(timbre, currenttarget.Amplitude);
            SortedSet<Metric> paningMetric = GetSortedMetric(paning, currenttarget.Position);

            if (cpos >= 1) {
                cpos = 0;
                WriteData(timbre[timbreMetric[0].Index], paning[paningMetric[0].Index]);
            }

            // sending out a progress report for visual feedback
            listeners.call(&Listener::BreedComplete,
                Listener::BreedCompleteData{
                    timbre[timbreMetric[0].Index].GetSpectrum(),
                    currenttarget.Amplitude,
                    paning[paningMetric[0].Index].GetLocation(),
                    currenttarget.Position,
                    timbre[timbreMetric[0].Index],
                    paning[paningMetric[0].Index],
                    timbreMetric[0].Metric,
                    paningMetric[0].Metric,
                    breed + loop,
                    ident });

            timbre = BreedPopulation(timbreMetric, population[breed], breedingFactor);
            paning = BreedPopulation(paningMetric, population[breed], breedingFactor);

            // defining the mutation inline since the method
            // definition would have been larger than the text
            for (int i = 0; i < mutationNumber[breed]; i++) {
                int target = gen->GetDouble(0, timbre.size());
                timbre[target].Mutate(mutationAmount[breed], frequencyWeighting);
                paning[target].Mutate(panningMutation[breed]);
            }

            if (threadShouldExit()) {
                return;
            }
        }
    }
}

std::vector<Biology::Gene> GeneController::InitializePopulation(int size, bool timbreMode)
{
    std::vector<Biology::Gene> out;
    for (int i = 0; i < size; i++) {
        Biology::Gene input(bandSize, framesPerGene, timbreMode, gen);
        out.emplace_back(input);
    }
    return out;
}

SortedSet<Metric> GeneController::GetSortedMetric(std::vector<Biology::Gene>& input,
                                                  const std::vector<double>& arg) const
{
    SortedSet<Metric> out;
    for (int i = 0; i < input.size(); i++) {
        out.add(Metric{ i, input[i].GetMetric(arg), input[i] });
    }
    return out;
}

SortedSet<Metric> GeneController::GetSortedMetric(std::vector<Biology::Gene>& input,
                                                  const std::complex<double>& arg) const
{
    SortedSet<Metric> out;
    for (int i = 0; i < input.size(); i++) {
        out.add(Metric{ i, input[i].GetMetric(arg), input[i] });
    }
    return out;
}

void GeneController::WriteData(const Biology::Gene& timbre, const Biology::Gene& paning)
{
    for (int i = 0; i < timbre.GetNumFrames(); i++) {
        for (int t = 0; t < timbre.GetFrame(i).GetData().size(); t++) {
            data->writeDouble(timbre.GetFrame(i).GetData()[t].real());
            data->writeDouble(timbre.GetFrame(i).GetData()[t].imag());
        }
        for (int t = 0; t < paning.GetFrame(i).GetData().size(); t++) {
            data->writeDouble(paning.GetFrame(i).GetData()[t].real());
            data->writeDouble(paning.GetFrame(i).GetData()[t].imag());
        }
    }
}

std::vector<Biology::Gene> GeneController::BreedPopulation(const SortedSet<Metric>& metric,
    int targetPopulation,
    int factor) const
{
    std::vector<Biology::Gene> output;
    for (int i = 0; i < targetPopulation; i++) {
        int motherID = gen->GetWeightedInt(0, metric.size(), factor);
        int fatherID = gen->GetWeightedInt(0, metric.size(), factor);
        Biology::Gene input(metric[motherID].Gene, metric[fatherID].Gene);
        output.emplace_back(input);
    }
    return output;
}