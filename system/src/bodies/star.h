#pragma once

#include <math/colour/colour.h>
#include <math/rng/prng.h>
#include <system/body.h>

#include "../quantity/galactic.h"
#include "../quantity/solar.h"

namespace galaxias
{
namespace system
{

class Star : public IBody
{
public:
    enum class Classification
    {
        W,
        O,
        B,
        A,
        F,
        G,
        K,
        M,
        L,
        T,
        Y,
    };

    Star(math::rng::Random&& dice);
    ~Star() override {}

    qty::Kilogram mass() const override { return mass_.base(); }

    SolarRadius radius() const { return radius_; }

    Classification classification() const;

    qty::Kelvin temperature() const { return temperature_; }

    /// Star colour assumed from black body temperature
    math::Colour colour() const;

    SolarLuminosity luminosity() const { return luminosity_; }

    qty::Metre sphereOfInfluence() const override;

    BolometricMagnitude absoluteMagnitude() const;

    ApparentMagnitude apparentMagnitude(const Parsec& distance) const;

private:
    /// Mass (prime value. All other derive from this one +- some noise)
    const SolarMass mass_;

    /// Radius of the star (useful for display)
    const SolarRadius radius_;

    /// Effective temperature (black body)
    const qty::BoundedKelvin temperature_;

    /// Luminosity (for absolute -> apparent magnitude)
    const SolarLuminosity luminosity_;
};

} // namespace system
} // namespace galaxias
