/*
  ==============================================================================

    FFTBin.cpp
    Created: 5 Aug 2015 10:43:12am
    Author:  Paul Popat

  ==============================================================================
*/

#include "ComplexFrame.h"

Biology::ComplexFrame::ComplexFrame(int Size, Utilities::Random *gen)
{
    this->gen = gen;
    for (int i = 0; i < Size; i++)
    {
        data.emplace_back(std::complex<double>{gen->GetDouble(-1, 1), gen->GetDouble(-1, 1)});
    }
}

std::complex<double> Biology::ComplexFrame::GetAveragePanning() const
{
    std::complex<double> output{0, 0};
    for (int d = 0; d < data.size(); d++)
    {
        output += data[d] / static_cast<double>(data.size());
    }
    return output;
}

void Biology::ComplexFrame::Mutate(double Amount,
                                   const std::vector<double> &Weighting)
{
    int t = gen->GetInt(0, data.size());
    std::complex<double> &r = data[t];
    double weighting = std::pow(Weighting[t], 2);

    if (gen->GetBool())
    {
        r += std::complex<double>{gen->GetDouble(-1, 0) * Amount * weighting,
                                  gen->GetDouble(0, 1) * Amount * weighting};
    }
    else
    {
        r += std::complex<double>{gen->GetDouble(0, 1) * Amount * weighting,
                                  gen->GetDouble(-1, 0) * Amount * weighting};
    }
}

void Biology::ComplexFrame::Mutate(double Amount)
{
    int t = gen->GetInt(0, data.size());
    std::complex<double> &r = data[t];

    auto clamp = [](auto i) {
        return std::min(std::max(i, -1.0), 1.0);
    };

    r = std::complex<double>{
        clamp(r.real() + gen->GetDouble(-1, 1) * Amount),
        clamp(r.imag() + gen->GetDouble(-1, 1) * Amount)};
}

const std::vector<std::complex<double>> &Biology::ComplexFrame::GetData() const
{
    return data;
}

int Biology::ComplexFrame::GetFrameSize() const { return data.size(); }