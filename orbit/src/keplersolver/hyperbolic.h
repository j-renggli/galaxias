#pragma once

#include "solver.h"

namespace galaxias
{
namespace orbit
{

class HyperbolicKeplerSolver : public UniversalKeplerSolver
{
public:
    using UniversalKeplerSolver::UniversalKeplerSolver;

    double f(double s) const override
    {
        const double s2 = sinh(sb_ * s * 0.5);
        const double c2 = cosh(sb_ * s * 0.5);

        return (2. * s2 * (c2 * (sb_ * r0_ + k_ / sb_) + rdotv_ * s2) + k_ * s) / -beta_ - h_;
    }

    double df(double s) const override
    {
        const double s2 = sinh(sb_ * s * 0.5);
        const double c2 = cosh(sb_ * s * 0.5);

        return df(s2, c2);
    }

    Factors factorsAt(double s) const override
    {
        const double s2 = sinh(sb_ * s * 0.5);
        const double c2 = cosh(sb_ * s * 0.5);
        const double r = df(s2, c2);

        const double twoS2 = 2. * s2;
        const double num = twoS2 * k_ * s2;

        return {1. + num / (r0_ * beta_),                           // f
                twoS2 * ((r0_ * c2 / sb_) - (rdotv_ * s2 / beta_)), // g
                -twoS2 * k_ * c2 / (r0_ * r * sb_),                 // f'
                1. + num / (r * beta_)};                            // g'
    }

private:
    double df(double s2, double c2) const
    {
        return r0_ + 2. * rdotv_ * s2 * c2 / sb_ + 2. * s2 * s2 * (k_ / beta_ - r0_);
    }

    math::Range<double> bisectionRange() const override
    {
        return math::Range<double>::make(-10. * guess_, 10. * guess_);
    }
};

} // namespace orbit
} // namespace galaxias
