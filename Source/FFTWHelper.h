//
//  FFTWHelper.hpp
//  SimpleGenetics
//
//  Created by Reuben Thomas on 24/02/2016.
//
//

#ifndef FFTWHelper_hpp
#define FFTWHelper_hpp

#include "fftw3.h"

#include <memory>

namespace FFTW
{
struct SelfDestructPlan
{
    SelfDestructPlan() = default;
    SelfDestructPlan(const fftw_plan &plan);
    virtual ~SelfDestructPlan() noexcept;
    operator const fftw_plan &() const;

  private:
    fftw_plan plan;
};

struct fftw_ptr_destructor
{
    template <typename T>
    void operator()(T t) const noexcept
    {
        fftw_free(t);
    }
};

using fftw_r = std::unique_ptr<double, fftw_ptr_destructor>;
using fftw_c = std::unique_ptr<fftw_complex, fftw_ptr_destructor>;
}

#endif /* FFTWHelper_hpp */
