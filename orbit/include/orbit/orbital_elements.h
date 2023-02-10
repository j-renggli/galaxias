#pragma once

#include "position.h"
#include <math/bounded_quantity.h>
#include <math/quantity.h>

namespace galaxias
{
namespace orbit
{

namespace qty = math::quantity;
namespace ratio = math::unit::ratio;

class OrbitalElements
{
public:
    /// Construct orbital elements and also provide the standard gravitational
    /// parameter this orbit is centered around to precompute the orbital period
    OrbitalElements(const qty::Unitless& eccentricity,
                    const qty::PerMetre& alpha, // 1 / semi-major axis
                    const qty::Radian& inclination,
                    const qty::Radian& longitude,
                    const qty::Radian& periapsis);

    /// Eccentricity
    qty::BoundedUnitless eccentricity_;

    /// 1 / Semi-major axis
    qty::PerMetre alpha_;

    /// Inclination
    qty::BoundedRadian inclination_;

    /// Longitude of ascending node
    qty::BoundedRadian longitude_;

    /// Argument of periapsis
    qty::BoundedRadian periapsis_;
};

} // namespace orbit
} // namespace galaxias
