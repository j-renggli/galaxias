#include <math/analytic_roots.h>

namespace galaxias
{
namespace math
{

/// Solve ax + b = 0
double linearRoot(double a, double b)
{
    if (a == 0.)
    {
        throw std::runtime_error("Constant value does not have meaningful roots");
    }

    return -b / a;
}

/// Solve ax2 + bx + c = 0
double firstRealQuadraticRoot(double a, double b, double c)
{
    if (a == 0.)
    {
        return linearRoot(b, c);
    }

    const double s = b * b - 4. * a * c;
    if (s < 0.)
    {
        throw std::runtime_error("No real root for this quadratic");
    }

    // Let's arbitrarily return the positive root...
    return (-b + sqrt(s)) / (2. * a);
}

/// Cardano's method for cubic root finding. Only the first, real root, is returned
double firstRealCubicRoot(double a, double b, double c, double d)
{
    if (a == 0.)
    {
        return firstRealQuadraticRoot(b, c, d);
    }

    // First, let's reduce by a
    b /= a;
    c /= a;
    d /= a;

    const double q = (b * b - 3. * c) / 9.;
    const double q3 = q * q * q;
    const double r = (2. * b * b * b + 27. * d - 9. * b * c) / 54.;
    const double r2 = r * r;

    if (r2 < q3)
    {
        // 3 real roots, only return the 1st...
        const double theta = acos(r / sqrt(q3));
        return -2. * sqrt(q) * cos(theta / 3.) - b / 3.;
    }

    const double w = sqrt(r2 - q3);
    const double m = pow(std::abs(r) + w, 1. / 3.); // Either s or t, without sign (due to cubic root)
    if (m == 0.)
    {
        return -b / 3.;
    }

    const double n = q / m; // t or s, depending on what m was
    return -sign(r) * (m + n) - b / 3.;
}

} // namespace math
} // namespace galaxias
