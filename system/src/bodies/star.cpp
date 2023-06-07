#include "star.h"

#include <math/rng/prng.h>

namespace galaxias
{
namespace system
{

namespace
{

using Rng = math::rng::Random;
constexpr auto zeroOne = math::Range<double>::zeroOne();

double generateMass(Rng& dice)
{
    // Generate the mass. VERY IMPORTANT as everything else depends on it
    // Current formula: -0.5*X + 0.11 * X^-0.41 + 0.465 but lim(x -> 0) = +inf so we clamp to 100-200
    const double x = dice.uniform(zeroOne);
    return std::min(100., -0.5 * x + 0.11 * std::pow(x, -0.41) + 0.465);
}

double generateRadius(const SolarMass& mass, Rng& dice)
{
    // Radius is correlated to mass with simplified formula 1.13 * M^0.63
    const double x = dice.gaussian(0., 0.2);
    return (1.13 + x) * std::pow(mass.value(), 0.63);
}

} // namespace

Star::Star(Rng&& dice)
    : mass_{generateMass(dice)}
    , radius_{generateRadius(mass_, dice)}
{
}

qty::Metre Star::sphereOfInfluence() const { return qty::Metre{-1.}; }

} // namespace system
} // namespace galaxias
