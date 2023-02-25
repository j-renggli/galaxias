#pragma once

#include "solver.h"

namespace galaxias
{
namespace orbit
{

class ParabolicKeplerSolver : public UniversalKeplerSolver
{
public:
    using UniversalKeplerSolver::UniversalKeplerSolver;

    double f(double s) const override
    {
        // Pure analytical case => Guess should be right immediately
        return s * (r0_ + 0.5 * r0_ * v0_ * s + k_ * s * s / 6.) - h_;
    }

    double df(double s) const override { return r0_ + r0_ * v0_ * s + 0.5 * k_ * s * s; }

    Factors factorsAt(double s) const override
    {
        const double r = df(s);

        return {
            1. - 0.5 * k_ * s * s / r0_,    // f
            r0_ * s * (1. + 0.5 * v0_ * s), // g
            -k_ * s / (r * r0_),            // f'
            r0_ * (1. + v0_ * s) / r        // g'
        };
    }

private:
    math::Range<double> bisectionRange() const override
    {
        assert(false);
        return math::Range<double>::make(0.5 * guess_, 1.5 * guess_);
    }
};

} // namespace orbit
} // namespace galaxias
