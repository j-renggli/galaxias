#pragma once

#include "../range.h"
#include "function.h"

namespace galaxias
{
namespace math
{
namespace solver
{

class ISolver
{
public:
    virtual ~ISolver() {}

    double findRoot(const IFunction& fct, const Range<double>& range, const double tolerance = 1e-7) const
    {
        return rootOf(fct, range, tolerance);
    }

private:
    virtual double rootOf(const IFunction& fct, const Range<double>& range, const double tolerance) const = 0;
};

// Custom exceptions

struct ConvergenceException : public std::exception
{
    ConvergenceException(size_t numIterations)
        : iterations_{numIterations}
    {
    }

    size_t iterations_;
};

struct IntervalException : public std::exception
{
};

} // namespace solver
} // namespace math
} // namespace galaxias
