#include "FFTWHelper.h"

namespace FFTW {
SelfDestructPlan::SelfDestructPlan(const fftw_plan& plan)
    : plan(plan)
{
}
SelfDestructPlan::~SelfDestructPlan() noexcept
{
    fftw_destroy_plan(plan);
}
SelfDestructPlan::operator const fftw_plan&() const
{
    return plan;
}
}