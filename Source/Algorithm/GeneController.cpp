/*
  ==============================================================================

    GeneController.cpp
    Created: 5 Aug 2015 12:00:06pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "GeneController.h"

GeneController::GeneController(const Settings &settings, File path, Utilities::Random *random)
    : Thread("Gene Thread")
{
    this->set = GetSettingsData(settings);
    this->pool = new JobPool(this->set.Threads);
    if (path.existsAsFile())
        path.deleteFile();
    path.create();
    this->data = path.createOutputStream();
    this->data->writeInt(this->set.FFTSize);
    this->data->writeInt(this->set.FFTSize);
    this->gen = random;
}

void GeneController::AddListener(Listener *l) { listeners.add(l); }

void GeneController::run()
{
    this->pool->run();
    BreedData d;
    d.Candidates = this->InitializePopulation(set.Population[0]);
    d.CurrentTarget = GetTarget(0);

    for (d.Loop = 0; d.Loop < set.CalculationLoops; d.Loop++)
    {
        for (d.Breed = 0; d.Breed < set.BreedingLoops; d.Breed++)
        {
            d.CurrentTarget = GetTarget(d.Breed);

            d.Metrics.clear();
            for (auto &candidate : d.Candidates)
            {
                this->pool->AddTestJob(&candidate, &d.CurrentTarget.Amplitude, &d.CurrentTarget.Position);
            }
            d.Metrics = this->pool->GetMetrics();

            d.CapturePosition += 1 / set.CaptureInterval[d.Breed];
            if (d.CapturePosition >= 1)
            {
                d.CapturePosition = 0;
                WriteData(d.Metrics[0].Timbre, d.Metrics[0].Panning);
            }

            // sending out a progress report for visual feedback
            listeners.call(&Listener::BreedComplete, d, set);

            d.Candidates.clear();
            for (int i = 0; i < set.Population[d.Breed]; i++)
            {
                auto motherID = this->gen->GetWeightedInt(0, d.Metrics.size(), this->set.BreedingFactor);
                auto fatherID = this->gen->GetWeightedInt(0, d.Metrics.size(), this->set.BreedingFactor);
                this->pool->AddBreedJob(&d.Metrics.getReference(motherID), &d.Metrics.getReference(fatherID));
            }
            d.Candidates = this->pool->GetCandidates();

            // defining the mutation inline since the method
            // definition would have been larger than the text
            for (int i = 0; i < set.MutationNumber[d.Breed]; i++)
            {
                int target = this->gen->GetInt(0, d.Candidates.size());
                this->pool->AddMutationJob(&d.Candidates[target],
                                           &this->set.MutationAmount[d.Breed],
                                           &this->set.PanningMutation[d.Breed],
                                           &this->set.FrequencyWeighting);
            }
            this->pool->WaitForMutation();
            this->pool->Clear();

            if (threadShouldExit())
            {
                return;
            }
        }
    }
}

std::vector<Candidate> GeneController::InitializePopulation(int size)
{
    std::vector<Candidate> out;
    for (int i = 0; i < size; i++)
    {
        out.emplace_back(
            Candidate(Biology::Gene(this->set.FFTSize, this->set.FramesPerGene, true, this->gen),
                      Biology::Gene(this->set.FFTSize, this->set.FramesPerGene, false, this->gen)));
    }
    return out;
}

void GeneController::WriteData(const Biology::Gene &timbre, const Biology::Gene &paning)
{
    for (int i = 0; i < timbre.GetNumFrames(); i++)
    {
        for (int t = 0; t < timbre.GetFrame(i).GetData().size(); t++)
        {
            data->writeDouble(timbre.GetFrame(i).GetData()[t].real());
            data->writeDouble(timbre.GetFrame(i).GetData()[t].imag());
        }
        for (int t = 0; t < paning.GetFrame(i).GetData().size(); t++)
        {
            data->writeDouble(paning.GetFrame(i).GetData()[t].real());
            data->writeDouble(paning.GetFrame(i).GetData()[t].imag());
        }
    }
}

FFTW::AudioAnalysis GeneController::GetTarget(int Breed) const
{
    for (auto &a : set.AudioBin)
    {
        if (set.TargetIdent[Breed] == a.Name)
        {
            return a;
        }
    }
    jassertfalse;
    return FFTW::AudioAnalysis();
}