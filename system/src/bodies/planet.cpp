#include "planet.h"

#include <math/rng/prng.h>

namespace galaxias
{
namespace system
{

namespace
{

using Rng = math::rng::Random;
constexpr auto zeroOne = math::Range<double>::zeroOne();
constexpr double aRocky{0.7};
constexpr double bRocky{0.63};
constexpr double aVolatile{1.03};
constexpr double bVolatile{0.29};
static const double inflexion = std::pow(aRocky / aVolatile, 1. / (bVolatile - bRocky));

double generateMass(Rng& dice)
{
    // Generate the mass. VERY IMPORTANT as everything else depends on it
    // Current formula: 0. < log(Mearth) < 100.
    const double x = dice.uniform(zeroOne);
    return std::max(1e-2, std::pow(10, 2. * x));
}

double generateRadius(const EarthMass& mass, Rng& dice)
{
    // Radius is correlated to mass with simplified formula
    // For mass < inflexion: 0.7 * x ^ 0.63
    // For mass > inflexion: 1.03 * x ^ 0.29
    const double x = dice.gaussian(0., 0.2);
    return (1.13 + x) * std::pow(mass.value(), 0.63);
}

} // namespace

Planet::Planet(Rng&& dice)
    : mass_{generateMass(dice)}
    , radius_{generateRadius(mass_, dice)}
{
    // /// Generate a star-specific parameter -> For what?
    // double parameter = dice.getUniformDouble() * 10000.;
}

qty::Metre Planet::sphereOfInfluence() const { return qty::Metre{-1.}; }

} // namespace system
} // namespace galaxias
