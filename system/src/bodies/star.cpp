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

double generateTemperature(const SolarMass& mass, Rng& dice)
{
    // Temperature (black body) is correlated to mass with simplified formula 6800 * M^0.62 - 850
    const double x = dice.gaussian<double>();
    return (6800 + x) * std::pow(mass.value(), 0.62) - 850.;
}

double generateLuminosity(const SolarRadius& radius, const qty::BoundedKelvin& temperature, Rng& dice)
{
    // Luminosity is correlated to radius and temperature with simplified formula R^2 * (T/Tsun)^4
    // See also https://en.wikipedia.org/wiki/Absolute_magnitude#Bolometric_magnitude
    // And https://en.wikipedia.org/wiki/Apparent_magnitude#Magnitude_addition
    const double x = dice.uniform<double>(math::Range{0.95, 1.05});
    return x * std::pow(radius.value(), 2.) * std::pow(temperature.value() / 5778., 4.);
}

} // namespace

Star::Star(Rng&& dice)
    : mass_{generateMass(dice)}
    , radius_{generateRadius(mass_, dice)}
    , temperature_{generateTemperature(mass_, dice), math::Range<double>::positive()}
    , luminosity_{generateLuminosity(radius_, temperature_, dice)}
{
}

BolometricMagnitude Star::absoluteMagnitude() const
{
    return BolometricMagnitude{-2.5 * log10(luminosity_.base().value()) + 71.197425};
}

ApparentMagnitude Star::apparentMagnitude(const Parsec& distance) const
{
    return ApparentMagnitude{absoluteMagnitude().value() + 5. * log10(distance.value()) - 5.};
}

math::Colour Star::colour() const { return math::blackBodyColour(temperature_); }

qty::Metre Star::sphereOfInfluence() const { return qty::Metre{-1.}; }

} // namespace system
} // namespace galaxias
