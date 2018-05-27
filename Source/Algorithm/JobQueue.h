/*
  ==============================================================================

    JobQueue.h
    Created: 27 May 2018 8:00:22am
    Author:  paul

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <complex>
#include "BreedData.h"

struct Job
{
  String Action;
  Candidate *ToTest;
  Metric *Mother;
  Metric *Father;
  Candidate *ToMutate;
  std::vector<double> *Amplitude;
  std::complex<double> *Panning;

  double *MutationAmount;
  double *PanningMutationAmount;
  std::vector<double> *FrequencyWeighting;
};

class Queue : public Thread
{
public:
  Queue(const Queue &q);
  Queue(int count);
  void AddJob(Job job);
  void run() override;
  bool Waiting() const;

  std::vector<Metric> *GetMetrics();
  std::vector<Candidate> *GetCandidates();
  void Clear();

private:
  std::vector<Metric> metrics;
  std::vector<Candidate> candidates;
  std::vector<Job> jobs;
  ReadWriteLock lock;
};

class JobPool
{
public:
  JobPool(int threads);
  void run();
  void Kill();
  void AddTestJob(Candidate *toTest,
                  std::vector<double> *amplitude,
                  std::complex<double> *panning);
  void AddBreedJob(Metric *mother,
                   Metric *father);
  void AddMutationJob(Candidate *toMutate,
                      double *timbreAmount,
                      double *panningAmount,
                      std::vector<double> *weighting);
  SortedSet<Metric> GetMetrics();
  std::vector<Candidate> GetCandidates();
  void WaitForMutation() const;
  void Clear();

private:
  void WaitForFinish() const;
  int current = 0;
  std::vector<ScopedPointer<Queue>> queues;
};