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
struct Metric {
    int Index;
    double Metric;
    Biology::Gene Gene;
    
    bool operator==(const struct Metric& rhs) const { return Metric == rhs.Metric; }
    bool operator<(const struct Metric& rhs) const { return Metric < rhs.Metric; }
    bool operator>(const struct Metric& rhs) const { return Metric > rhs.Metric; }
    bool operator<=(const struct Metric& rhs) const { return Metric <= rhs.Metric; }
    bool operator>=(const struct Metric& rhs) const { return Metric >= rhs.Metric; }
};

/** A simple struct to hold all the settings from the config */
struct SettingsData {
    int FFTSize;
    int FramesPerGene;
    int BreedingLoops;
    int CalculationLoops;
    int BreedingFactor;
    int BandSize;
    
    int FrequencyBand;
    
    std::vector<double> CaptureInterval;
    std::vector<double> MutationAmount;
    std::vector<double> MutationNumber;
    std::vector<double> Population;
    std::vector<double> FrequencyWeighting;
    std::vector<double> PanningMutation;
    std::vector<String> TargetIdent;
    std::vector<FFTW::AudioAnalysis> AudioBin;
};

/** A simple struct to hold all of the data relevant to the breeding process
 this can then be passed with ease */
struct BreedData {
    std::vector<Biology::Gene> Timbre;
    std::vector<Biology::Gene> Panning;
    
    SortedSet<Metric> TimbreMetric;
    SortedSet<Metric> PanningMetric;
    int Loop = 0;
    int Breed = 0;
    FFTW::AudioAnalysis CurrentTarget;
    double CapturePosition = 0;
};

/** The controller class will be in charge of creating the population, gathering the metrics.
 breeding and mutations.*/
class GeneController : public Thread {
public:
    GeneController(const Settings& Settings, File Path, int Band, Utilities::Random* Gen);

    /** The Listener is for classes that want to recieve data from the algorithm as it is running */
    class Listener {
    public:
        virtual ~Listener() = default;
        // this method is going to be called a lot. Don't do anything intensive here
        virtual void BreedComplete(const BreedData& data, const SettingsData & settings) = 0;
    };

    /** does what you expect a simple way to get updates on progress */
    void AddListener(Listener* l);
    /** this is where the meat of the algorithm is done. call startThread() instead for multithreading */
    void run() override;

private:
    /** creates a brand new population of a given size */
    std::vector<Biology::Gene> InitializePopulation(int size, bool timbreMode);
    /** returns a sorted metric for a timbre gene */
    SortedSet<Metric> GetSortedMetric(std::vector<Biology::Gene>& input, const std::vector<double>& arg) const;
    /** returns a sorted metric for a panning gene */
    SortedSet<Metric> GetSortedMetric(std::vector<Biology::Gene>& input, const std::complex<double>& arg) const;
    /** writes the timbre and panning data to a bin to be read by the AudioWriter class later */
    void WriteData(const Biology::Gene& timbre, const Biology::Gene& paning);
    /** takes the population with the metrics attached and breeds them together for a shiney new one! */
    std::vector<Biology::Gene> BreedPopulation(const SortedSet<Metric>& metric, int targetPopulation, int factor) const;
    
    /** Returns the desired target audio file for that point in time */
    FFTW::AudioAnalysis GetTarget(int Breed) const;

    /** the bin file for writing to */
    ScopedPointer<FileOutputStream> data;

    /** listeners for the updates */
    ListenerList<Listener> listeners;

    /** random generator for the population to use */
    Utilities::Random* gen;
    
    SettingsData set;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeneController)
};

#endif // GENECONTROLLER_H_INCLUDED
