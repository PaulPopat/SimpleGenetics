/*
  ==============================================================================

    PopMember.cpp
    Created: 5 Aug 2015 10:41:00am
    Author:  Paul Popat

  ==============================================================================
*/

#include "Gene.h"

Biology::Gene::Gene(const Gene & Mother, const Gene & Father) {
    gen = Mother.gen;
    timbreMode = Mother.timbreMode;
    int XOver = gen->GetInt(0, Mother.GetNumFrames());
    for (int x = 0 ; x < XOver ; x++) {
        data.add(ComplexFrame(Mother.data[x]));
    }
    for (int x = XOver ; x < Father.GetNumFrames() ; x++) {
        data.add(ComplexFrame(Father.data[x]));
    }
    CalculateSpectrumOrLocation();
}

Biology::Gene::Gene(int FrameLength, int NumFrames, bool TimbreMode, Utilities::Random * Gen) {
    gen = Gen;
    for (int i = 0 ; i < NumFrames ; i++) {
        ComplexFrame f(FrameLength, gen);
        data.add(f);
    }
    timbreMode = TimbreMode;
    CalculateSpectrumOrLocation();
}

double Biology::Gene::GetMetric(const Array<double> & Target) const {
    if (!timbreMode) return 0;
    double metric = 0;

    for (int i = 0 ; i < spectrum.size() ; i++) {
        metric += std::pow(std::abs(std::log10(spectrum[i]) - std::log10(Target[i])), 4);
    }
    
    return metric;
}

double Biology::Gene::GetMetric(const FFT::Complex & Target) const {
    if (timbreMode) return 0;
    double metric = 0;
    
    metric += std::abs(location.r - Target.r);
    metric += std::abs(location.i - Target.i);
    return metric;
}

void Biology::Gene::Mutate(double Amount, const Array<double> Weighting) {
    int b = gen->GetInt(0, data.size());
    data.getReference(b).Mutate(Amount, Weighting);
}

void Biology::Gene::Mutate(double Amount) {
    int b = gen->GetInt(0, data.size());
    data.getReference(b).Mutate(Amount);
}

int Biology::Gene::GetNumFrames() const { return data.size(); }
const Array<double> & Biology::Gene::GetSpectrum() const { return spectrum; }
const FFT::Complex & Biology::Gene::GetLocation() const { return location; }
const Biology::ComplexFrame & Biology::Gene::GetFrame(int i) const { return data.getReference(i); }

void Biology::Gene::CalculateSpectrumOrLocation() {
    if (timbreMode) {
        
        spectrum.resize(data[0].GetFrameSize());
        for (int i = 0 ; i < spectrum.size() ; i++)
            spectrum.getReference(i) = 0;
        
        for (int d = 0 ; d < data.size() ; d++) {
            const Array<FFT::Complex> & frame = data.getReference(d).GetData();
            for (int i = 0 ; i < frame.size() ; i++) {
                spectrum.getReference(i) += std::sqrt(std::pow(frame[i].r, 2) + std::pow(frame[i].i, 2)) / data.size();
            }
        }
        
    }
    else {
    
        location = FFT::Complex{0, 0};
        for (int d = 0 ; d < data.size() ; d++) {
            location.r += data.getReference(d).GetAveragePanning().r / data.size();
            location.i += data.getReference(d).GetAveragePanning().i / data.size();
        }
        
    }
}