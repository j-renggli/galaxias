#include <math/solver/newton_raphson.h>

namespace galaxias
{
namespace math
{
namespace solver
{

double NewtonRaphson::rootOf(const IFunction& fct, const Range<double>& range, const double tolerance) const
{
    return NewtonRaphson::findRoot(fct, range.mid(), tolerance);
}

double NewtonRaphson::findRoot(const IFunction& fct, double x, const double tolerance)
{
    constexpr size_t max{50}; // TODO: user-defined max steps
    for (size_t i = 0; i < max; ++i)
    {
        const double y = fct.f(x);
        const double dy = fct.df(x);

        constexpr double epsilon{1e-15};
        if (std::abs(dy) < epsilon)
        {
            if (y < epsilon)
            {
                return x;
            }
            break;
        }

        const double x1 = x - y / dy;

        if (std::abs(x1 - x) <= tolerance)
        {
            return x1;
        }

        x = x1;
    }

    throw ConvergenceException(max /*"Failed to converge after " + std::to_string(max) + " iterations"*/);
}

} // namespace solver
} // namespace math
} // namespace galaxias
