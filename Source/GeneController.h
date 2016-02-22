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
#include "AudioLoader.h"
#include "BandCalculator.h"
#include "Gene.h"
#include "RandomGen.h"
#include "Settings.h"

/** A simple contain for holding a metric and an index so that the Gene with that
 metric can be accessed later*/
class Metric {
public:
    int Index;
    double Metric;
};

inline bool operator==(const Metric& l, const Metric& r) { return l.Metric == r.Metric; }
inline bool operator<(const Metric& l, const Metric& r) { return l.Metric < r.Metric; }
inline bool operator>(const Metric& l, const Metric& r) { return l.Metric > r.Metric; }
inline bool operator<=(const Metric& l, const Metric& r) { return l.Metric <= r.Metric; }
inline bool operator>=(const Metric& l, const Metric& r) { return l.Metric >= r.Metric; }

/** The controller class will be in charge of creating the population, gathering the metrics.
 breeding and mutations.*/
class GeneController : public Thread {
public:
    GeneController(const Settings& Settings, File Path, int Band, Utilities::Random* Gen);

    /** The Listener is for classes that want to recieve data from the algorithm as it is running */
    class Listener {
    public:
        virtual ~Listener() = default;
        /** a simple way to transfer the data. Allowing for expansion at a later data
         without breaking other classes */
        struct BreedCompleteData {
            const Array<double>& amplitude;
            const Array<double>& target;
            const FFT::Complex& position;
            const FFT::Complex& targetPos;
            const Biology::Gene& timbreData;
            const Biology::Gene& panningData;
            const double& timbreMetric;
            const double& panningMetric;
            const int& framesComplete;
            int ident;
        };
        // this method is going to be called a lot. Don't do anything intensive here
        virtual void BreedComplete(const BreedCompleteData& data) = 0;
    };

    /** does what you expect a simple way to get updates on progress */
    void AddListener(Listener* l);
    /** this is where the meat of the algorithm is done. call startThread() instead for multithreading */
    void run() override;

private:
    /** creates a brand new population of a given size */
    Array<Biology::Gene> InitializePopulation(int size, bool timbreMode);
    /** returns a sorted metric for a timbre gene */
    SortedSet<Metric> GetSortedMetric(Array<Biology::Gene>& input, const Array<double>& arg);
    /** returns a sorted metric for a panning gene */
    SortedSet<Metric> GetSortedMetric(Array<Biology::Gene>& input, const FFT::Complex& arg);
    /** writes the timbre and panning data to a bin to be read by the AudioWriter class later */
    void WriteData(const Biology::Gene& timbre, const Biology::Gene& paning);
    /** takes the population with the metrics attached and breeds them together for a shiney new one! */
    Array<Biology::Gene> BreedPopulation(const Array<Biology::Gene>& population,
        const SortedSet<Metric>& metric,
        int targetPopulation, int factor);

    /** the bin file for writing to */
    ScopedPointer<FileOutputStream> data;

    /** listeners for the updates */
    ListenerList<Listener> listeners;
    /** frequency band number to identify on call listener calls */
    int ident;
    /** random generator for the population to use */
    Utilities::Random* gen;

    // all the relevent data from the settings while the algorithm is running
    int fftSize;
    int framesPerGene;
    int breedingLoops;
    int calculationLoops;
    int breedingFactor;
    int bandSize;

    Array<double> captureInterval;
    Array<double> mutationAmount;
    Array<double> mutationNumber;
    Array<double> population;
    Array<double> frequencyWeighting;
    Array<double> panningMutation;
    Array<int> targetIdent;
    Array<FFTW::AudioAnalysis> target;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeneController)
};

#endif // GENECONTROLLER_H_INCLUDED
