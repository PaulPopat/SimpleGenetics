/*
  ==============================================================================

    FFTBin.cpp
    Created: 5 Aug 2015 10:43:12am
    Author:  Paul Popat

  ==============================================================================
*/

#include "ComplexFrame.h"

Biology::ComplexFrame::ComplexFrame(int Size, Utilities::Random* Gen)
{
    gen = Gen;
    for (int i = 0; i < Size; i++) {
        data.add(std::complex<double>{ gen->GetDouble(-1, 1), gen->GetDouble(-1, 1) });
    }
}

std::complex<double> Biology::ComplexFrame::GetAveragePanning() const
{
    std::complex<double> output{ 0, 0 };
    for (int d = 0; d < data.size(); d++) {
        output += data.getReference(d) / static_cast<double>(data.size());

    }
    return output;
}

void Biology::ComplexFrame::Mutate(double Amount,
    const Array<double>& Weighting)
{
    int t = gen->GetInt(0, data.size());
    std::complex<double>& r = data.getReference(t);

    if (gen->GetBool()) {
        r += std::complex<double>{gen->GetDouble(-1, 0) * Amount * Weighting[t],
                                  gen->GetDouble(0, 1) * Amount * Weighting[t]};
    }
    else {
        r += std::complex<double>{gen->GetDouble(0, 1) * Amount * Weighting[t],
                                  gen->GetDouble(-1, 0) * Amount * Weighting[t]};
    }
}

void Biology::ComplexFrame::Mutate(double Amount)
{
    int t = gen->GetInt(0, data.size());
    std::complex<double>& r = data.getReference(t);

    auto clamp = [](auto i) {
        return std::min(std::max(i, -1.0), 1.0);
    };

    r = std::complex<double>{
        clamp(r.real() + gen->GetDouble(-1, 1) * Amount),
        clamp(r.imag() + gen->GetDouble(-1, 1) * Amount)
    };
}

const Array<std::complex<double>>& Biology::ComplexFrame::GetData() const
{
    return data;
}

int Biology::ComplexFrame::GetFrameSize() const { return data.size(); }