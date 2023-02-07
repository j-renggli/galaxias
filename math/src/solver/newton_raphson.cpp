#include <math/solver/newton_raphson.h>

namespace galaxias
{
namespace math
{
namespace solver
{

double NewtonRaphson::findRoot(const IFunction& fct, const Range<double>& range, const double tolerance) const
{
    double x = range.mid();
    constexpr size_t max{50}; // TODO: user-defined max steps
    for (size_t i = 0; i < max; ++i)
    {
        const double y = fct.f(x);
        const double dy = fct.df(x);

        // TODO: Check abs(dy) is large enough else throw

        const double x1 = x - y / dy;

        if (std::abs(x1 - x) <= tolerance)
        {
            return x1;
        }

        x = x1;
    }

    throw std::runtime_error("Failed to converge after " + std::to_string(max) + " iterations");
}

} // namespace solver
} // namespace math
} // namespace galaxias
