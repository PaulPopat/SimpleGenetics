/*
  ==============================================================================

    BreedData.h
    Created: 27 May 2018 8:00:33am
    Author:  paul

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Gene.h"
#include "../Utilities/AudioLoader.h"

struct Candidate
{
  Candidate(Biology::Gene timbre, Biology::Gene panning)
  {
    this->Timbre = timbre;
    this->Panning = panning;
  }

  Biology::Gene Timbre;
  Biology::Gene Panning;
};

/** A simple contain for holding a metric and an index so that the Gene with that
 metric can be accessed later*/
struct Metric
{
  Metric() = default;
  Metric(double value, Candidate candidate)
  {
    this->Value = value;
    this->Timbre = candidate.Timbre;
    this->Panning = candidate.Panning;
  }

  Metric(const Metric &m)
  {
    this->Value = m.Value;
    this->Timbre = m.Timbre;
    this->Panning = m.Panning;
  }

  double Value;
  Biology::Gene Timbre;
  Biology::Gene Panning;

  bool operator==(const struct Metric &rhs) const { return Value == rhs.Value; }
  bool operator<(const struct Metric &rhs) const { return Value < rhs.Value; }
  bool operator>(const struct Metric &rhs) const { return Value > rhs.Value; }
  bool operator<=(const struct Metric &rhs) const { return Value <= rhs.Value; }
  bool operator>=(const struct Metric &rhs) const { return Value >= rhs.Value; }
};

/** A simple struct to hold all of the data relevant to the breeding process
 this can then be passed with ease */
struct BreedData
{
  std::vector<Candidate> Candidates;

  SortedSet<Metric> Metrics;
  int Loop = 0;
  int Breed = 0;
  FFTW::AudioAnalysis CurrentTarget;
  double CapturePosition = 0;
};
