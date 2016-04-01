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
    set.FFTSize = Settings.GetIntValue("FFTSize");
    set.FramesPerGene = Settings.GetIntValue("FramesPerGene");
    set.BreedingLoops = Settings.GetIntValue("BreedingLoops");
    set.CalculationLoops = Settings.GetIntValue("CalculationLoops");
    set.BreedingFactor = Settings.GetIntValue("BreedingFactor");

    set.BandSize = Utilities::GetBandSize(Settings.GetIntValue("FrequencyBands"), Band, set.FFTSize);

    set.CaptureInterval = Settings.GetGraph("CaptureInterval", set.BreedingLoops);
    set.MutationAmount = Settings.GetGraph("MutationAmount", set.BreedingLoops);
    set.MutationNumber = Settings.GetGraph("MutationNumber", set.BreedingLoops);
    set.Population = Settings.GetGraph("Population", set.BreedingLoops);
    set.PanningMutation = Settings.GetGraph("PanningMutation", set.BreedingLoops);
    set.TargetIdent = Settings.GetAudioGraph("Target", set.BreedingLoops);
    set.AudioBin = Settings.GetAudioData("Target", set.FFTSize, Settings.GetIntValue("FrequencyBands"), Band);

    set.FrequencyWeighting = Utilities::SplitToBand(Settings.GetGraph("FrequencyWeighting", set.FFTSize),
        Settings.GetIntValue("FrequencyBands"), Band);

    if (Path.existsAsFile())
        Path.deleteFile();
    Path.create();
    data = Path.createOutputStream();
    data->writeInt(set.FFTSize);
    data->writeInt(set.BandSize);

    set.FrequencyBand = Band;
    gen = Gen;
}

void GeneController::AddListener(Listener* l) { listeners.add(l); }

void GeneController::run()
{
    BreedData d;
    d.Timbre = InitializePopulation(set.Population[0], true);
    d.Panning = InitializePopulation(set.Population[0], false);
    d.CurrentTarget = GetTarget(0);

    for (d.Loop = 0; d.Loop < set.CalculationLoops; d.Loop++) {
        for (d.Breed = 0; d.Breed < set.BreedingLoops; d.Breed++) {
            
            d.CurrentTarget = GetTarget(d.Breed);

            d.TimbreMetric = GetSortedMetric(d.Timbre, d.CurrentTarget.Amplitude);
            d.PanningMetric = GetSortedMetric(d.Panning, d.CurrentTarget.Position);

            d.CapturePosition += 1 / set.CaptureInterval[d.Breed];
            if (d.CapturePosition >= 1) {
                d.CapturePosition = 0;
                WriteData(d.Timbre[d.TimbreMetric[0].Index], d.Panning[d.PanningMetric[0].Index]);
            }

            // sending out a progress report for visual feedback
            listeners.call(&Listener::BreedComplete, d, set);

            d.Timbre = BreedPopulation(d.TimbreMetric, set.Population[d.Breed], set.BreedingFactor);
            d.Panning = BreedPopulation(d.PanningMetric, set.Population[d.Breed], set.BreedingFactor);

            // defining the mutation inline since the method
            // definition would have been larger than the text
            for (int i = 0; i < set.MutationNumber[d.Breed]; i++) {
                int target = gen->GetInt(0, d.Timbre.size());
                d.Timbre[target].Mutate(set.MutationAmount[d.Breed], set.FrequencyWeighting);
                d.Panning[target].Mutate(set.PanningMutation[d.Breed]);
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
        Biology::Gene input(set.BandSize, set.FramesPerGene, timbreMode, gen);
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

FFTW::AudioAnalysis GeneController::GetTarget(int Breed) const {
    for ( const auto & t : set.TargetIdent ) {
        for ( auto & a : set.AudioBin ) {
            if ( t == a.Name ) {
                return a;
            }
        }
    }
    jassertfalse;
    return FFTW::AudioAnalysis();
}