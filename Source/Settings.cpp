/*
  ==============================================================================

    Settings.cpp
    Created: 5 Aug 2015 12:07:13pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "Settings.h"
using namespace std;

void Settings::LoadTargets() {
    for (int f = 0 ; f < Target.size() ; f++) {
        vector<double> TargetBinSummed (FFTSize, 0);
        if (Target[f].first != "Silence") {
            File *input = new File(workingDir->getFullPathName() + "/" + Target[f].first);
            vector<double> InputAudio = LoadFile(input);
            delete input;
            InputAudio = NormalizeAudio(InputAudio);
            TargetBinSummed = GetBin(InputAudio, FFTSize);
            TargetBinSummed = NormalizeLoudness(TargetBinSummed, 2);
        }
        Targets.push_back(TargetBinSummed);
    }
}

Settings::Settings() {
    FFTSize = 4096;
    WaveSize = 20;
    WaveCount = 50000;
    SampleRate = 44100;
    Channels = 2;
    Factor = 3;
    CaptureInterval.push_back(make_pair(1, 0));
    Target.push_back(make_pair("Silence", 0));
    MutationAmount.push_back(make_pair(0.5, 0));
    MutationChance.push_back(make_pair(0.2, 0));
    Population.push_back(make_pair(10, 0));
    workingDir = new File(File::getCurrentWorkingDirectory());
    LoadTargets();
}

Settings::Settings(File *Path) {
    vector<string> settingsData = GetFileData(Path);
    int i = 0;
    while (i < settingsData.size()) {
        vector<string> line = StringSplit(settingsData[i]);
        if (line[0] == "FFTSize") FFTSize = atoi(line[1].c_str());
        else if (line[0] == "BinsPerFrame") WaveSize = atoi(line[1].c_str());
        else if (line[0] == "Factor") Factor = atoi(line[1].c_str());
        else if (line[0] == "FramesToCalculate") WaveCount = atoi(line[1].c_str());
        else if (line[0] == "SampleRate") SampleRate = atoi(line[1].c_str());
        else if (line[0] == "Channels") Channels = atoi(line[1].c_str());
        else if (line[0] == "CaptureInterval") CaptureInterval = LoadVectorSettings(i, settingsData);
        else if (line[0] == "TargetFile") Target = LoadTargetSettings(i, settingsData);
        else if (line[0] == "MutationAmount") MutationAmount = LoadVectorSettings(i, settingsData);
        else if (line[0] == "MutationChance") MutationChance = LoadVectorSettings(i, settingsData);
        else if (line[0] == "PopulationSize") Population = LoadVectorSettings(i, settingsData);
        i++;
    }
    workingDir = new File(Path->getParentDirectory());
    LoadTargets();
}

Settings::~Settings() {
    delete workingDir;
}

int Settings::GetPopulation(double Position) {
    return Interpolate(Position, Population);
}

int Settings::GetMutationChance(double Position) {
    return Interpolate(Position, MutationChance);
}

double Settings::GetMutationAmount(double Position) {
    return Interpolate(Position, MutationAmount);
}

double Settings::GetCaptureInterval(double Position) {
    return Interpolate(Position, CaptureInterval);
}

vector<double> Settings::GetTarget(double Position) {
    for (int i = Target.size() - 1 ; i >= 0 ; i--) {
        if (Position >= Target[i].second) {
            return Targets[i];
        }
    }
    return Targets[0];
}

void Settings::Save(File *Path) {
    ofstream f;
    String path = Path->getFullPathName();
    f.open(path.toRawUTF8(), fstream::out | fstream::trunc);
    
    f << "FFTSize " << FFTSize << endl;
    f << "BinsPerFrame " << WaveSize << endl;
    f << "FramesToCalculate " << WaveCount << endl;
    f << "SampleRate " << SampleRate << endl;
    f << "Channels " << Channels << endl;
    f << "Factor " << Factor << endl;
    
    f << "CapterInterval" << endl;
    for (int i = 0 ; i < CaptureInterval.size() ; i++) {
        f << CaptureInterval[i].first << " " << CaptureInterval[i].second << endl;
    }
    f << "end" << endl;
    
    f << "TargetFile" << endl;
    for (int i = 0 ; i < Target.size() ; i++) {
        f << Target[i].first << " " << Target[i].second << endl;
        CopyAudio(File(workingDir->getFullPathName() + "/" + Target[i].first), Path->getParentDirectory());
    }
    f << "end" << endl;
    
    f << "MutationAmount" << endl;
    for (int i = 0 ; i < MutationAmount.size() ; i++) {
        f << MutationAmount[i].first << " " << MutationAmount[i].second << endl;
    }
    f << "end" << endl;
    
    f << "MutationChance" << endl;
    for (int i = 0 ; i < MutationChance.size() ; i++) {
        f << MutationChance[i].first << " " << MutationChance[i].second << endl;
    }
    f << "end" << endl;
    
    f << "PopulationSize" << endl;
    for (int i = 0 ; i < Population.size() ; i++) {
        f << Population[i].first << " " << Population[i].second << endl;
    }
    f << "end" << endl;
    
    delete workingDir;
    workingDir = new File(Path->getParentDirectory());
    
    f.close();
}

