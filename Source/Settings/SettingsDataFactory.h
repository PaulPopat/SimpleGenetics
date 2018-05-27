/*
  ==============================================================================

    SettingsDataFactory.h
    Created: 26 May 2018 8:25:35am
    Author:  paul

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include "../Utilities/AudioLoader.h"
#include "../Settings/Settings.h"

struct SettingsData
{
  int FFTSize;
  int FramesPerGene;
  int BreedingLoops;
  int CalculationLoops;
  int BreedingFactor;
  int Threads;

  std::vector<double> CaptureInterval;
  std::vector<double> MutationAmount;
  std::vector<double> MutationNumber;
  std::vector<double> Population;
  std::vector<double> FrequencyWeighting;
  std::vector<double> PanningMutation;
  std::vector<String> TargetIdent;
  std::vector<FFTW::AudioAnalysis> AudioBin;
};

SettingsData GetSettingsData(const Settings &settings);
