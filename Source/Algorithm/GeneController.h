/*
  ==============================================================================

    GeneController.h
    Created: 5 Aug 2015 12:00:06pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef GENECONTROLLER_H_INCLUDED
#define GENECONTROLLER_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "../Utilities/AudioLoader.h"
#include "../Utilities/BandCalculator.h"
#include "Gene.h"
#include "BreedData.h"
#include "../Utilities/RandomGen.h"
#include "../Settings/Settings.h"
#include "../Settings/SettingsDataFactory.h"
#include "JobQueue.h"

/** The controller class will be in charge of creating the population, gathering the metrics.
 breeding and mutations.*/
class GeneController : public Thread
{
public:
  GeneController(const Settings &settings, File path, Utilities::Random *random);

  /** The Listener is for classes that want to recieve data from the algorithm as it is running */
  class Listener
  {
  public:
    virtual ~Listener() = default;
    // this method is going to be called a lot. Don't do anything intensive here
    virtual void BreedComplete(const BreedData &data, const SettingsData &settings) = 0;
  };

  /** does what you expect a simple way to get updates on progress */
  void AddListener(Listener *l);
  /** this is where the meat of the algorithm is done. call startThread() instead for multithreading */
  void run() override;

private:
  /** creates a brand new population of a given size */
  std::vector<Candidate> InitializePopulation(int size);
  /** writes the timbre and panning data to a bin to be read by the AudioWriter class later */
  void WriteData(const Biology::Gene &timbre, const Biology::Gene &paning);

  /** Returns the desired target audio file for that point in time */
  FFTW::AudioAnalysis GetTarget(int Breed) const;

  /** the bin file for writing to */
  ScopedPointer<FileOutputStream> data;

  /** listeners for the updates */
  ListenerList<Listener> listeners;

  /** random generator for the population to use */
  Utilities::Random *gen;

  SettingsData set;

  ScopedPointer<JobPool> pool;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeneController)
};

#endif // GENECONTROLLER_H_INCLUDED
