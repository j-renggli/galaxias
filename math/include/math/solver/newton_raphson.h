#pragma once

#include "solver.h"

namespace galaxias
{
namespace math
{
namespace solver
{

class NewtonRaphson : public ISolver
{
public:
    virtual ~NewtonRaphson() = default;

    /// @note The root finding will start at range.mid()
    double rootOf(const IFunction& fct, const Range<double>& range, const double tolerance = 1e-7) const override;

    static double findRoot(const IFunction& fct, double guess, const double tolerance = 1e-7);
};

} // namespace solver
} // namespace math
} // namespace galaxias
