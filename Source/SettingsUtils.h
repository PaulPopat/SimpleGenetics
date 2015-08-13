/*
  ==============================================================================

    SettingsUtils.h
    Created: 6 Aug 2015 9:16:27am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef SETTINGSUTILS_H_INCLUDED
#define SETTINGSUTILS_H_INCLUDED
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include "fftw3.h"
#include "../JuceLibraryCode/JuceHeader.h"

std::vector<std::string> GetFileData(File *Path);
std::vector<std::string> StringSplit(std::string Input);
std::vector<double> GetBin(std::vector<double> Input, int FFTSize);
std::vector<std::vector<double>> LoadFile(File *filepath);
std::vector<double> NormalizeAudio(std::vector<double> audio);
std::vector<double> NormalizeLoudness(std::vector<double> audio, double targetLoudness);
std::vector<std::pair<double,double> > LoadVectorSettings(int &i, std::vector<std::string> settingsData);
std::vector<std::pair<std::string,double> > LoadTargetSettings(int &i, std::vector<std::string> settingsData);
double LinearInterp(double Distance, double Value1, double Value2);
double Interpolate(double Position, std::vector<std::pair< double, double > > Data);
void CopyAudio(File InputPath, File OutputDir);
void WriteAudio(std::vector<std::vector<double> > audio, int sampleRate, File Path);

#endif  // SETTINGSUTILS_H_INCLUDED
