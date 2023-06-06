#include <orbit/orbital_elements.h>

#include <math/solver/newton_raphson.h>

namespace galaxias
{
namespace orbit
{

OrbitalElements::OrbitalElements(const qty::Unitless& eccentricity,
                                 const qty::PerMetre& alpha,
                                 const qty::Radian& inclination,
                                 const qty::Radian& longitude,
                                 const qty::Radian& periapsis)
    : eccentricity_{eccentricity, math::Range<double>::positive()}
    , alpha_{alpha}
    , inclination_{qty::BoundedRadian::fromModulo(inclination, math::Range<double>{0., M_PI})}
    , longitude_{qty::BoundedRadian::fromModulo(longitude, math::Range<double>::radians())}
    , periapsis_{qty::BoundedRadian::fromModulo(periapsis, math::Range<double>::radians())}
{
}

} // namespace orbit
} // namespace galaxias
