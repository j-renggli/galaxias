#pragma once

#include "solver.h"

namespace galaxias
{
namespace math
{
namespace solver
{

class Brent : public ISolver
{
public:
    virtual ~Brent() = default;

    double rootOf(const IFunction& fct, const Range<double>& range, const double tolerance = 1e-7) const override;

    static double findRoot(const IFunction& fct, const Range<double>& range, const double tolerance = 1e-7);
};

} // namespace solver
} // namespace math
} // namespace galaxias
