/*
  ==============================================================================

    SettingsDataFactory.cpp
    Created: 26 May 2018 8:31:36am
    Author:  paul

  ==============================================================================
*/

#include "SettingsDataFactory.h"

SettingsData GetSettingsData(const Settings &settings)
{
  SettingsData result;
  result.FFTSize = settings.GetIntValue("FFTSize");
  result.FramesPerGene = settings.GetIntValue("FramesPerGene");
  result.BreedingLoops = settings.GetIntValue("BreedingLoops");
  result.CalculationLoops = settings.GetIntValue("CalculationLoops");
  result.BreedingFactor = settings.GetIntValue("BreedingFactor");
  result.Threads = settings.GetIntValue("Threads");

  result.CaptureInterval = settings.GetGraph("CaptureInterval", result.BreedingLoops);
  result.MutationAmount = settings.GetGraph("MutationAmount", result.BreedingLoops);
  result.MutationNumber = settings.GetGraph("MutationNumber", result.BreedingLoops);
  result.Population = settings.GetGraph("Population", result.BreedingLoops);
  result.PanningMutation = settings.GetGraph("PanningMutation", result.BreedingLoops);
  result.TargetIdent = settings.GetAudioGraph("Target", result.BreedingLoops);
  result.AudioBin = settings.GetAudioData("Target", result.FFTSize);

  result.FrequencyWeighting = settings.GetGraph("FrequencyWeighting", result.FFTSize);
  return result;
}
