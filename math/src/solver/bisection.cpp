#include <math/solver/bisection.h>

namespace galaxias
{
namespace math
{
namespace solver
{

double Bisection::findRoot(const IFunction& fct,
                           const Range<double>& range,
                           const double tolerance) const
{
    double x0 = range.low();
    const double y0 = fct.f(x0);
    double x1 = range.high();
    const double y1 = fct.f(x1);
    const double tol2 = tolerance * tolerance;

    if (y0 * y0 <= tol2)
    {
        return x0;
    }
    if (y1 * y1 <= tol2)
    {
        return x1;
    }

    if (y0 * y1 > 0.)
    {
        throw std::runtime_error("Both limits evaluate to positive or "
                                 "negative, won't search for a root here");
    }

    double x = x1; // Arbitrary
    double y = y1;
    while (y * y > tol2)
    {
        x = (x0 + x1) * 0.5;
        if (x == x0 || x == x1)
        {
            throw std::runtime_error("No root in range");
            return x;
        }

        y = fct.f(x);

        if (y * y0 > 0.)
        {
            x0 = x;
        }
        else
        {
            x1 = x;
        }
    }

    return x;
}

} // namespace solver
} // namespace math
} // namespace galaxias
