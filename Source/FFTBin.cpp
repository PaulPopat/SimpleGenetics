/*
  ==============================================================================

    FFTBin.cpp
    Created: 5 Aug 2015 10:43:12am
    Author:  Paul Popat

  ==============================================================================
*/

#include "FFTBin.h"
using namespace std;

double RandomVal(double min, double max, double weighting) {
    return pow((double)rand() / (double)RAND_MAX, weighting) * (max - min) + min;
}

FFTBin::FFTBin(int size) {
    for (int i = 0 ; i < size ; i++) {
        data.push_back(make_pair(0, 0));
    }
}

FFTBin::FFTBin(vector<FFTBin*> others) {
    for (int i = 0 ; i < others.size() ; i++) {
        for (int x = 0 ; x < others[i]->data.size() ; x++) {
            data.push_back(make_pair(others[i]->data[x].first, others[i]->data[x].second));
        }
    }
}

FFTBin::FFTBin(FFTBin* other) {
    for (int i = 0 ; i < other->data.size() ; i++) {
        data.push_back(make_pair(other->data[i].first, other->data[i].second));
    }
}

FFTBin::~FFTBin() {
    data.clear();
}

vector<double> FFTBin::GetMagnitudes() {
    vector<double> output;
    for (int i = 0 ; i < data.size() ; i++) {
        output.push_back(sqrt(pow(data[i].first, 2) + pow(data[i].second, 2)));
    }
    return output;
}

void FFTBin::Mutate(double amount) {
    int t = RandomVal(0, data.size(), 1);
    data[t].first += RandomVal(-1, 1, 1) * amount;
    data[t].second += RandomVal(-1, 1, 1) * amount;
}

vector< pair<double, double> > FFTBin::GetData() {
    return data;
}

int FFTBin::GetBinSize() {
    return data.size();
}
