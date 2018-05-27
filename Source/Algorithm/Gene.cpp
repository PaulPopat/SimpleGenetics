/*
  ==============================================================================

    PopMember.cpp
    Created: 5 Aug 2015 10:41:00am
    Author:  Paul Popat

  ==============================================================================
*/

#include "Gene.h"

Biology::Gene::Gene(const Gene &Mother, const Gene &Father)
{
    gen = Mother.gen;
    timbreMode = Mother.timbreMode;
    int XOver = gen->GetInt(0, Mother.GetNumFrames());
    for (int x = 0; x < XOver; x++)
    {
        data.emplace_back(ComplexFrame(Mother.data[x]));
    }
    for (int x = XOver; x < Father.GetNumFrames(); x++)
    {
        data.emplace_back(ComplexFrame(Father.data[x]));
    }
    CalculateSpectrumOrLocation();
}

Biology::Gene::Gene(int FrameLength, int NumFrames, bool TimbreMode, Utilities::Random *Gen)
{
    gen = Gen;
    for (int i = 0; i < NumFrames; i++)
    {
        ComplexFrame f(FrameLength, gen);
        data.emplace_back(f);
    }
    timbreMode = TimbreMode;
    CalculateSpectrumOrLocation();
}

double Biology::Gene::GetMetric(const std::vector<double> &Target) const
{
    if (!timbreMode)
        return 0;
    double metric = 0;

    for (int i = 0; i < spectrum.size(); i++)
    {
        metric += std::pow(std::abs(std::log10(spectrum[i]) - std::log10(Target[i])), 4);
    }

    return metric;
}

double Biology::Gene::GetMetric(const std::complex<double> &Target) const
{
    if (timbreMode)
        return 0;
    double metric = 0;

    metric += std::abs(location.real() - Target.real());
    metric += std::abs(location.imag() - Target.imag());
    return metric;
}

void Biology::Gene::Mutate(double Amount, const std::vector<double> Weighting)
{
    int b = gen->GetInt(0, data.size());
    data[b].Mutate(Amount, Weighting);
}

void Biology::Gene::Mutate(double Amount)
{
    int b = gen->GetInt(0, data.size());
    data[b].Mutate(Amount);
}

int Biology::Gene::GetNumFrames() const { return data.size(); }
const std::vector<double> &Biology::Gene::GetSpectrum() const { return spectrum; }
const std::complex<double> &Biology::Gene::GetLocation() const { return location; }
const Biology::ComplexFrame &Biology::Gene::GetFrame(int i) const { return data[i]; }

void Biology::Gene::CalculateSpectrumOrLocation()
{
    if (timbreMode)
    {

        spectrum.resize(data[0].GetFrameSize(), 0);

        for (int d = 0; d < data.size(); d++)
        {
            const std::vector<std::complex<double>> &frame = data[d].GetData();
            for (int i = 0; i < frame.size(); i++)
            {
                spectrum[i] += std::sqrt(std::pow(frame[i].real(), 2) + std::pow(frame[i].imag(), 2)) / data.size();
            }
        }
    }
    else
    {

        location = std::complex<double>{0, 0};
        for (int d = 0; d < data.size(); d++)
        {
            location += data[d].GetAveragePanning() / static_cast<double>(data.size());
        }
    }
}