/*
  ==============================================================================

    Settings.h
    Created: 5 Aug 2015 12:07:13pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include "../JuceLibraryCode/JuceHeader.h"
#include "SettingsUtils.h"

class Settings{
public:
    Settings();
    Settings(File *Path);
    ~Settings();
    void Save(File *Path);
    int FFTSize;
    int WaveSize;
    int WaveCount;
    int SampleRate;
    int Channels;
    int Factor;
    int GetPopulation(double Position);
    int GetMutationChance(double Position);
    double GetMutationAmount(double Position);
    std::vector<double> GetTarget(double Position);
    double GetCaptureInterval(double Position);
    
    std::vector< std::pair< double, double > > Population;
    std::vector< std::pair< double, double > > MutationChance;
    std::vector< std::pair< double, double > > MutationAmount;
    std::vector< std::pair< std::string, double > > Target;
    std::vector< std::vector < double > > Targets;
    std::vector< std::pair< double, double > > CaptureInterval;
private:
    void LoadTargets();
    File *workingDir;
};


#endif  // SETTINGS_H_INCLUDED
