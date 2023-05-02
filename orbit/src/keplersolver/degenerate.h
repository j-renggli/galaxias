#pragma once

#include "solver.h"

namespace galaxias
{
namespace orbit
{

/// A Kepler solver... for central body (r=0, v=0)
class ZeroKeplerSolver : public UniversalKeplerSolver
{
public:
    using UniversalKeplerSolver::UniversalKeplerSolver;

    double f(double s) const override
    {
        // Pure analytical case => Guess should be right immediately
        return s; // * (0.5 * rdotv_ * s + k_ * s * s / 6.) - h_;
    }

    double df(double s) const override { return 1.; }

    Factors factorsAt(double s) const override
    {
        return {
            1., // f
            0., // g
            0., // f'
            1.  // g'
        };
    }

private:
    math::Range<double> bisectionRange() const override
    {
        assert(false);
        return math::Range<double>::make(-1., 1.);
    }
};

} // namespace orbit
} // namespace galaxias
