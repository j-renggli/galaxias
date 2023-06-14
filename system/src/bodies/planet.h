#pragma once

#include <math/rng/prng.h>
#include <system/body.h>

#include "../quantity/earth.h"

namespace galaxias
{
namespace system
{

class Planet : public IBody
{
public:
    Planet(math::rng::Random&& dice);
    ~Planet() override {}

    qty::Kilogram mass() const override { return mass_.base(); }

    qty::Metre radius() const { return radius_.base(); }

    qty::Metre sphereOfInfluence() const override;

private:
    /// Mass (prime value. All other derive from this one +- some noise)
    const EarthMass mass_;

    /// Radius of the star (useful for display)
    const EarthRadius radius_;
};

} // namespace system
} // namespace galaxias
