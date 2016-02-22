/*
  ==============================================================================

    GeneController.cpp
    Created: 5 Aug 2015 12:00:06pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "GeneController.h"

GeneController::GeneController(const Settings & Settings, File Path, int Band,  Utilities::Random * Gen) :
Thread("Gene Thread " + String(Band)) {
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
    
    if (Path.existsAsFile()) Path.deleteFile();
    Path.create();
    data = Path.createOutputStream();
    data->writeInt(fftSize);
    data->writeInt(bandSize);
    
    ident = Band;
    gen = Gen;
}

void GeneController::AddListener(Listener *l) { listeners.add(l); }

void GeneController::run() {
    Array<Biology::Gene> timbre = InitializePopulation(population[0], true);
    Array<Biology::Gene> paning = InitializePopulation(population[0], false);
    
    // setting up memory space for this loop
    FFTW::AudioAnalysis & currenttarget = target.getReference(0); // for putting the current target in
    double cpos = 0; // capture interval is added to this and when it reaches 1 a snapshot is taken
    
    for (int loop = 0 ; loop < calculationLoops ; loop++) {
        for (int breed = 0 ; breed < breedingLoops ; breed++) {
            
            cpos += 1 / captureInterval[breed];
            currenttarget = target.getReference(targetIdent[breed]);
            
            SortedSet<Metric> timbreMetric = GetSortedMetric(timbre, currenttarget.Amplitude);
            SortedSet<Metric> paningMetric = GetSortedMetric(paning, currenttarget.Position);
            
            if (cpos >= 1) {
                cpos = 0;
                WriteData(timbre.getReference(timbreMetric[0].Index), paning.getReference(paningMetric[0].Index));
            }
            
            // sending out a progress report for visual feedback
            listeners.call(&Listener::BreedComplete,
                            Listener::BreedCompleteData{
                                timbre.getReference(timbreMetric[0].Index).GetSpectrum(),
                                currenttarget.Amplitude,
                                paning.getReference(paningMetric[0].Index).GetLocation(),
                                currenttarget.Position,
                                timbre.getReference(timbreMetric[0].Index),
                                paning.getReference(paningMetric[0].Index),
                                timbreMetric.getReference(0).Metric,
                                paningMetric.getReference(0).Metric,
                                breed + loop,
                                ident});
            
            Array<Biology::Gene> ttemp = BreedPopulation(timbre, timbreMetric, population[breed], breedingFactor);
            Array<Biology::Gene> ptemp = BreedPopulation(paning, paningMetric, population[breed], breedingFactor);
            timbre = ttemp;
            paning = ptemp;
            
            // defining the mutation inline since the method
            // definition would have been larger than the text
            for (int i = 0 ; i < mutationNumber[breed] ; i++) {
                int target = gen->GetDouble(0, timbre.size());
                timbre.getReference(target).Mutate(mutationAmount[breed], frequencyWeighting);
                paning.getReference(target).Mutate(panningMutation[breed]);
            }
            
            if (threadShouldExit()) {
                return;
            }
        }
    }
}

Array<Biology::Gene> GeneController::InitializePopulation(int size, bool timbreMode) {
    Array<Biology::Gene> out;
    for (int i = 0 ; i < size ; i++) {
        Biology::Gene input(bandSize, framesPerGene, timbreMode, gen);
        out.add(input);
    }
    return out;
}

SortedSet<Metric> GeneController::GetSortedMetric(Array<Biology::Gene> & input, const Array<double> & arg) {
    SortedSet<Metric> out;
    for (int i = 0 ; i < input.size() ; i++) {
        out.add(Metric{i, input.getReference(i).GetMetric(arg)});
    }
    return out;
}

SortedSet<Metric> GeneController::GetSortedMetric(Array<Biology::Gene> & input, const FFT::Complex & arg) {
    SortedSet<Metric> out;
    for (int i = 0 ; i < input.size() ; i++) {
        out.add(Metric{i, input.getReference(i).GetMetric(arg)});
    }
    return out;
}

void GeneController::WriteData(const Biology::Gene &timbre, const Biology::Gene &paning) {
    for (int i = 0 ; i < timbre.GetNumFrames() ; i++) {
        for (int t = 0 ; t < timbre.GetFrame(i).GetData().size() ; t++) {
            data->writeFloat(timbre.GetFrame(i).GetData().getUnchecked(t).r);
            data->writeFloat(timbre.GetFrame(i).GetData().getUnchecked(t).i);
        }
        for (int t = 0 ; t < paning.GetFrame(i).GetData().size() ; t++) {
            data->writeFloat(paning.GetFrame(i).GetData().getUnchecked(t).r);
            data->writeFloat(paning.GetFrame(i).GetData().getUnchecked(t).i);
        }
    }
}

Array<Biology::Gene> GeneController::BreedPopulation(const Array<Biology::Gene> & population,
                                           const SortedSet<Metric> &metric,
                                           int targetPopulation,
                                           int factor) {
    Array<Biology::Gene> output;
    for (int i = 0 ; i < targetPopulation ; i++) {
        int motherID = gen->GetWeightedInt(0, metric.size(), factor);
        int fatherID = gen->GetWeightedInt(0, metric.size(), factor);
        Biology::Gene input(population[metric[motherID].Index], population[metric[fatherID].Index]);
        output.add(input);
    }
    return output;
}