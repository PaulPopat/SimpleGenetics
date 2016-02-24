/*
  ==============================================================================

    PopMember.h
    Created: 5 Aug 2015 10:41:00am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef POPMEMBER_H_INCLUDED
#define POPMEMBER_H_INCLUDED
#include "ComplexFrame.h"
#include "RandomGen.h"

namespace Biology {

class Gene {
public:
    /** default for arrays and such. Greats an empty object so not very useful */
    Gene() {}
    /** COPIES data into this gene to breed. Takes some from mother and some from father.
     Also takes timbre mode bool from the mother. */
    Gene(const Gene& Mother, const Gene& Father);
    /** creates new data with the random generator pointer. stores generator but
     does not take owner ship so keep it in scope!*/
    Gene(int FrameLength, int NumFrames, bool TimbreMode, Utilities::Random* Gen);
    /** if you are using this for timbre call this for the metric. if it is in
     panning mode rather than timbre then this will return nothing */
    double GetMetric(const Array<double>& Target) const;
    /** if you are using this for panning call this for the metric. if it is in
     timbre mode rather than panning then this will return nothing */
    double GetMetric(const ComplexDouble& Target) const;
    /** mutates with a weighting for how much each band should be affected.
     weighting must be at least the size of a frame */
    void Mutate(double Amount, const Array<double> Weighting);
    /** mutates one band randomly */
    void Mutate(double Amount);
    /** returns the number of frames in this gene */
    int GetNumFrames() const;
    /** returns the average band amplitude. will do nothing if not in timbre mode */
    const Array<double>& GetSpectrum() const;
    /** returns the average location. will do nothing if in timbre mode */
    const ComplexDouble& GetLocation() const;
    /** returns a reference to the fft frame of given index */
    const ComplexFrame& GetFrame(int i) const;

private:
    void CalculateSpectrumOrLocation();
    Array<ComplexFrame> data;
    Array<double> spectrum;
    ComplexDouble location;
    Utilities::Random* gen;
    bool timbreMode;
};
}

#endif // POPMEMBER_H_INCLUDED
