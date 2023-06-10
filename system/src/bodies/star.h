#pragma once

#include <math/rng/prng.h>
#include <system/body.h>

#include "../quantity/solar.h"

namespace galaxias
{
namespace system
{

class Star : public IBody
{
public:
    Star(math::rng::Random&& dice);
    ~Star() override {}

    qty::Kilogram mass() const override { return mass_.base(); }

    qty::Metre radius() const { return radius_.base(); }

    qty::Metre sphereOfInfluence() const override;

private:
    /// Mass (prime value. All other derive from this one +- some noise)
    const SolarMass mass_;

    /// Radius of the star (useful for display)
    const SolarRadius radius_;

    /// Effective temperature (black body)
    const qty::BoundedKelvin temperature_;

    /// Luminosity (for absolute -> apparent magnitude)
    const qty::BoundedWatt luminosity_;
};

} // namespace system
} // namespace galaxias
