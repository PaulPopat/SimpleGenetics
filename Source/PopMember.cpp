/*
  ==============================================================================

    PopMember.cpp
    Created: 5 Aug 2015 10:41:00am
    Author:  Paul Popat

  ==============================================================================
*/

#include "PopMember.h"
using namespace std;

PopMember::PopMember() {
    
}

PopMember::PopMember(PopMember *Mother, PopMember *Father) {
    if (Mother->data.size() != Father->data.size()) return;
    for (int c = 0 ; c < Mother->data.size() ; c++) {
        data.push_back(vector<FFTBin*>());
        int XOver = RandomVal(0, Mother->data[c].size(), 1);
        for (int x = 0 ; x < XOver ; x++) {
            data[c].push_back(new FFTBin(Mother->data[c][x]));
        }
        for (int x = XOver ; x < Father->data[c].size() ; x++) {
            data[c].push_back(new FFTBin(Father->data[c][x]));
        }
    }
}

PopMember::PopMember(PopMember *Copy) {
    for (int c = 0 ; c < Copy->data.size() ; c++) {
        data.push_back(vector<FFTBin*>());
        for (int x = 0 ; x < Copy->data[c].size() ; x++) {
            data[c].push_back(new FFTBin(Copy->data[c][x]));
        }
    }
}

PopMember::PopMember(int BinLength, int NumBins, int NumChannels) {
    for (int c = 0 ; c < NumChannels ; c++) {
        data.push_back(vector<FFTBin*>());
        for (int i = 0 ; i < NumBins ; i++) {
            data[c].push_back(new FFTBin(BinLength));
        }
    }
}

PopMember::~PopMember() {
    for (int c = 0 ; c < data.size() ; c++) {
        for (int i = 0 ; i < data[c].size() ; i++) {
            delete data[c][i];
        }
    }
}

void PopMember::AddMember(PopMember *Addition) {
    for (int c = 0 ; c < Addition->data.size() ; c++) {
        if (data.size() < c + 1) data.push_back(vector<FFTBin*>());
        for (int i = 0 ; i < Addition->data[c].size() ; i++) {
            data[c].push_back(new FFTBin(Addition->data[c][i]));
        }
    }
}

double PopMember::GetCloseness(vector<double> Target) {
    double closeness = 0;
    vector<double> SummedMember(Target.size());
    for (int c = 0 ; c < data.size() ; c++) {
        for (int b = 0 ; b < data[c].size() ; b++) {
            vector<double> magnitude = data[c][b]->GetMagnitudes();
            for (int i = 0 ; i < magnitude.size() ; i++) {
                SummedMember[i] += magnitude[i] / data[c].size();
            }
        }
    }
    for (int i = 0 ; i < SummedMember.size() ; i++) {
        SummedMember[i] /= data.size();
        closeness += abs(SummedMember[i] - Target[i]) * (SummedMember.size() - i);
    }
    return closeness;
}

void PopMember::Mutate(double Amount) {
    int c = RandomVal(0, data.size(), 1);
    int b = RandomVal(0, data[c].size(), 1);
    data[c][b]->Mutate(Amount);
}


vector<vector<double> > PopMember::GetAudio() {
    int fftSize = data[0][0]->GetBinSize();
    vector<vector<double> > returnVal(data.size());
    FFT *fft = new FFT(log2(fftSize), true);
    FFT::Complex *input = new FFT::Complex[fftSize];
    FFT::Complex *output = new FFT::Complex[fftSize];
    for (int c = 0 ; c < data.size(); c++) {
        for (int i = 0 ; i < data[c].size() ; i++) {
            vector<pair<double, double> > audio = data[c][i]->GetData();
            for (int x = 0 ; x < fftSize ; x++) {
                input[x].r = audio[x].first;
                input[x].i = audio[x].second;
            }
            fft->perform(input, output);
            for (int x = 0 ; x < fftSize ; x++) {
                returnVal[c].push_back(output[x].r);
            }
        }
    }
    delete fft;
    delete input;
    delete output;
    return returnVal;
}

int PopMember::GetNumBins() {
    if (data.size() > 0) {
        return data[0].size();
    }
    return 0;
}
