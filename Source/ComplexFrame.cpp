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
        data.add(ComplexDouble{ gen->GetDouble(-1, 1), gen->GetDouble(-1, 1) });
    }
}

Biology::ComplexDouble Biology::ComplexFrame::GetAveragePanning() const
{
    ComplexDouble output{ 0, 0 };
    for (int d = 0; d < data.size(); d++) {
        output.r += data.getReference(d).r / data.size();
        output.i += data.getReference(d).i / data.size();
    }
    return output;
}

void Biology::ComplexFrame::Mutate(double Amount,
    const Array<double>& Weighting)
{
    int t = gen->GetInt(0, data.size());
    ComplexDouble& r = data.getReference(t);

    if (gen->GetBool()) {
        r.r += gen->GetDouble(-1, 0) * Amount * Weighting[t];
        r.i += gen->GetDouble(0, 1) * Amount * Weighting[t];
    }
    else {
        r.r += gen->GetDouble(0, 1) * Amount * Weighting[t];
        r.i += gen->GetDouble(-1, 0) * Amount * Weighting[t];
    }
}

void Biology::ComplexFrame::Mutate(double Amount)
{
    int t = gen->GetInt(0, data.size());
    ComplexDouble& r = data.getReference(t);

    r.r += gen->GetDouble(-1, 1) * Amount;
    if (r.r > 1)
        r.r = 1;
    else if (r.r < -1)
        r.r = -1;

    r.i += gen->GetDouble(-1, 1) * Amount;
    if (r.i > 1)
        r.i = 1;
    else if (r.i < -1)
        r.i = -1;
}

const Array<Biology::ComplexDouble>& Biology::ComplexFrame::GetData() const
{
    return data;
}

int Biology::ComplexFrame::GetFrameSize() const { return data.size(); }