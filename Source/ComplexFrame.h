/*
  ==============================================================================

    FFTBin.h
    Created: 5 Aug 2015 10:43:12am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef COMPLEXFRAME_H_INCLUDED
#define COMPLEXFRAME_H_INCLUDED
#include "RandomGen.h"

#include <complex>

namespace Biology {

class ComplexFrame {
public:
    /** default for arrays and such. Greats an empty object so not very useful */
    ComplexFrame() = default;
    /** creates new data with the random generator pointer. stores generator but
   does not take owner ship so keep it in scope!*/
    ComplexFrame(int Size, Utilities::Random* Gen);

    /** returns the average location of the frame on a 2d plane.
   assuming that each complex number is a coordinate not frequency data*/
    std::complex<double> GetAveragePanning() const;
    /** mutates with a weighting for how much each band should be affected.
   weighting must be at least the size of a frame */
    void Mutate(double Amount, const Array<double>& Weighting);
    /** mutates one band randomly */
    void Mutate(double Amount);
    /** returns the basic data for other uses */
    const Array<std::complex<double>>& GetData() const;
    /** returns the number of complex numbers stored */
    int GetFrameSize() const;

private:
    Array<std::complex<double>> data;
    Utilities::Random* gen;
};
};

#endif // COMPLEXFRAME_H_INCLUDED
