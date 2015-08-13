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
    settings = Setting;
    input = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * settings->FFTSize + 1);
    output = (double*) fftw_malloc(sizeof(double) * (settings->FFTSize * 2));
    ifft = fftw_plan_dft_c2r_1d(settings->FFTSize * 2, input, output, FFTW_ESTIMATE);
}

GeneController::~GeneController() {
    int popSize = population.size();
    for (int i = 0 ; i < popSize ; i++) {
        delete population[i];
    }
    delete final;
    fftw_destroy_plan(ifft);
    fftw_free(input);
    fftw_free(output);
}

void GeneController::run() {
    delete final;
    final = new PopMember;
    if (population.size() > 0) {
        for (int i = 0 ; i < population.size() ; i++) {
            delete population[i];
        }
        population.clear();
    }
    for (int i = 0 ; i < settings->GetPopulation(0) ; i++)
        population.push_back(new PopMember(settings->FFTSize, settings->WaveSize, settings->Channels));
    
    for (loc = 0 ; loc < settings->WaveCount ; loc++) {
        pos = (double)(loc + 1) / (double)settings->WaveCount;
        
        vector<pair<int, double> > Close;
        for (int p = 0 ; p < population.size() ; p++)
            Close.push_back(make_pair(p, population[p]->GetCloseness(settings->GetTarget(pos), settings)));
        sort(Close.begin(), Close.end(), Sorter);
        closeness = Close[0].second;
        
        cPos += settings->GetCaptureInterval(pos);
        if (cPos >= 1) {
            cPos = 0;
            final->AddMember(population[Close[0].first]);
        }
        vector<vector<double> > temp = population[Close[0].first]->GetAudio(input, output, ifft);
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
            population[p]->Mutate(settings->GetMutationAmount(pos), settings);
        }
        
        if (threadShouldExit())
            return;
    }
}

vector<vector<double> > GeneController::GetAudio() {return final->GetAudio(input, output, ifft);}

void GeneController::LoadSettings(Settings *Setting) {
    settings = Setting;
    fftw_destroy_plan(ifft);
    fftw_free(input);
    fftw_free(output);
    input = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * settings->FFTSize + 1);
    output = (double*) fftw_malloc(sizeof(double) * (settings->FFTSize * 2));
    ifft = fftw_plan_dft_c2r_1d(settings->FFTSize * 2, input, output, FFTW_ESTIMATE);
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