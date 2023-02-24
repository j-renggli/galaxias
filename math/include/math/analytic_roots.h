#pragma once

#include <cmath>
#include <stdexcept>

namespace galaxias
{
namespace math
{

template <typename T>
int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

/// Solve ax + b = 0
double linearRoot(double a, double b);

/// Solve ax2 + bx + c = 0
double firstRealQuadraticRoot(double a, double b, double c);

/// Cardano's method for cubic root finding. Only the first, real root, is returned
double firstRealCubicRoot(double a, double b, double c, double d);

} // namespace math
} // namespace galaxias
