/*
  ==============================================================================

    GeneController.cpp
    Created: 5 Aug 2015 12:00:06pm
    Author:  Paul Popat

  ==============================================================================
*/

#include "GeneController.h"
using namespace std;

GeneController::GeneController(Settings *Setting) : Thread("GeneThread") {
    settings = Setting;
}

GeneController::~GeneController() {
    int popSize = population.size();
    for (int i = 0 ; i < popSize ; i++) {
        delete population[i];
    }
    delete final;
}

void GeneController::run() {
    for (int i = 0 ; i < settings->GetPopulation(0) ; i++)
        population.push_back(new PopMember(settings->FFTSize, settings->WaveSize, settings->Channels));
    
    for (loc = 0 ; loc < settings->WaveCount ; loc++) {
        pos = (double)(loc + 1) / (double)settings->WaveCount;
        
        vector<pair<int, double> > Close;
        for (int p = 0 ; p < population.size() ; p++)
            Close.push_back(make_pair(p, population[p]->GetCloseness(settings->GetTarget(pos))));
        sort(Close.begin(), Close.end(), Sorter);
        closeness = Close;
        
        cPos += settings->GetCaptureInterval(pos);
        if (cPos >= 1) {
            cPos = 0;
            final->AddMember(population[closeness[0].first]);
        }
        
        vector<PopMember*> breed;
        for (int i = 0 ; i < settings->GetPopulation(pos) ; i++) {
            int tOne = closeness[RandomVal(0, population.size(), settings->Factor)].first;
            int tTwo = closeness[RandomVal(0, population.size(), settings->Factor)].first;
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

void GeneController::LoadSettings(Settings *Setting) {settings = Setting;}

double GeneController::GetPosition() {
    return pos;
}

double GeneController::GetCloseness() {
    if (closeness.size() > 0)
        return closeness[0].second;
    else
        return 0;
}

int GeneController::GetLoc() {
    return loc;
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