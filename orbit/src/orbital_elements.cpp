#include <orbit/orbital_elements.h>

#include <math/solver/newton_raphson.h>

namespace galaxias
{
namespace orbit
{

namespace
{

constexpr double pi{M_PI};
constexpr double two_pi{2. * pi};
constexpr double max{std::numeric_limits<double>::max()};
constexpr math::Range range_positive{0., max};
constexpr math::Range range_0_2pi{0., two_pi};

} // namespace

OrbitalElements::OrbitalElements(const qty::Unitless& eccentricity,
                                 const qty::PerMetre& alpha,
                                 const qty::Radian& inclination,
                                 const qty::Radian& longitude,
                                 const qty::Radian& periapsis)
    : eccentricity_{eccentricity, range_positive}
    , alpha_{alpha}
    , inclination_{qty::BoundedRadian::fromModulo(inclination, {0., pi})}
    , longitude_{qty::BoundedRadian::fromModulo(longitude, range_0_2pi)}
    , periapsis_{qty::BoundedRadian::fromModulo(periapsis, range_0_2pi)}
{
}

} // namespace orbit
} // namespace galaxias
