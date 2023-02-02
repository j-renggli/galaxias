#pragma once

#include <math/solver/function.h>

namespace galaxias
{
namespace math
{
namespace test
{

class Linear : public solver::IFunction
{
    virtual double f(double x) const override { return x; }
    virtual double df(double) const override { return 1.; }
};

class Quadratic : public solver::IFunction
{
    virtual double f(double x) const override { return x * x - 3. * x - 6; }
    virtual double df(double x) const override { return 2. * x - 3.; }
};

} // namespace test
} // namespace math
} // namespace galaxias
