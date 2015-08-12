/*
  ==============================================================================

    GeneController.cpp
    Created: 5 Aug 2015 12:00:06pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "GeneController.h"
using namespace std;

void SwapData(PopMember *valToFill, PopMember *newVal) {
    swap(valToFill, newVal);
    delete newVal;
}

GeneController::GeneController(Settings *Setting) : Thread("GeneThread") {
    settings = new Settings(Setting);
}

GeneController::~GeneController() {
    int popSize = population.size();
    for (int i = 0 ; i < popSize ; i++) {
        delete population[i];
    }
    delete final;
}

void GeneController::run() {
    delete final;
    final = new PopMember;
    for (int i = 0 ; i < settings->GetPopulation(0) ; i++)
        population.push_back(new PopMember(settings->FFTSize, settings->WaveSize, settings->Channels));
    
    for (loc = 0 ; loc < settings->WaveCount ; loc++) {
        pos = (double)(loc + 1) / (double)settings->WaveCount;
        
        vector<pair<int, double> > Close;
        for (int p = 0 ; p < population.size() ; p++)
            Close.push_back(make_pair(p, population[p]->GetCloseness(settings->GetTarget(pos))));
        sort(Close.begin(), Close.end(), Sorter);
        closeness = Close[0].second;
        
        cPos += settings->GetCaptureInterval(pos);
        if (cPos >= 1) {
            cPos = 0;
            final->AddMember(population[Close[0].first]);
        }
        vector<vector<double> > temp = population[Close[0].first]->GetAudio();
        mostRecent = temp;
        
        vector<PopMember*> breed;
        for (int i = 0 ; i < settings->GetPopulation(pos) ; i++) {
            int tOne = Close[RandomVal(0, population.size(), settings->Factor)].first;
            int tTwo = Close[RandomVal(0, population.size(), settings->Factor)].first;
            breed.push_back(new PopMember(population[tOne], population[tTwo]));
        }
        for (int p = 0 ; p < population.size() ; p++)
            delete population[p];
        population = breed;
        
        for (int m = 0 ; m < settings->GetMutationChance(pos) ; m++) {
            int p = RandomVal(0, population.size(), 1);
            population[p]->Mutate(settings->GetMutationAmount(pos));
        }
        
        if (threadShouldExit())
            return;
    }
}

vector<vector<double> > GeneController::GetAudio() {return final->GetAudio();}

void GeneController::LoadSettings(Settings *Setting) {
    delete settings;
    settings = new Settings(Setting);
}

double GeneController::GetPosition() {
    return pos;
}

double GeneController::GetCloseness() {
    return closeness;
}

int GeneController::GetLoc() {
    return loc;
}

vector<vector<double> > GeneController::GetCurrentAudio() {
    return mostRecent;
}

int GeneController::GetAudioLength() {
    return (final->GetNumBins() * settings->FFTSize) / settings->SampleRate;
}

double GeneController::RandomVal(double min, double max, double weighting) {
    return pow((double)rand() / (double)RAND_MAX, weighting) * (max - min) + min;
}

bool GeneController::RandomBool() {return rand() % 2;}

bool GeneController::Sorter(const pair<int, double> &left, const pair<int, double> &right) {
    // sorts the closeness index
    return left.second < right.second;
}