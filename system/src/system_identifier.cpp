#include <system/system_identifier.h>

#include "galactic_quantity.h"
#include <math/bounded_quantity.h>

namespace galaxias
{

namespace qty = math::quantity;

namespace system
{

namespace
{

constexpr math::Range<double> range01{0., 1.};
constexpr math::Range<double> rangePM1{-1., 1.};
constexpr math::Range<double> rangeRadian{0., 2. * M_PI};

constexpr uint64_t identifierShift{18};
constexpr uint32_t identifierMask = 0x3FFFF;
constexpr uint64_t coordsMask{0x2378A9CB3FEC95CULL};
constexpr double identifierMax = 262144.; // 0x40000 as integer

uint64_t valueFrom(uint32_t r, uint32_t phi)
{
    return (static_cast<uint64_t>(r) << identifierShift) + static_cast<uint64_t>(phi);
}

math::rng::Mersenne makeSystemDice(uint32_t r, uint32_t phi)
{
    /// First prepare a die. This is a very important step that influences the whole galaxy !
    /// Do not modify unless aware of the consequences !!! Make sure it works !!!
    const uint64_t mult = r * phi;
    const uint64_t x_or = r ^ phi;
    return math::rng::Mersenne{(mult * x_or) ^ valueFrom(r, phi)};
}

orbit::coordinates::GalactoCentric makeCoordinates(math::rng::Mersenne& dice, uint32_t rawAngle, uint32_t rawRadius)
{
    /// Some constants: (angle, radius) is the "raw" galactic coordinates as defined by the identifier where
    ///                 angle  = discrete pseudo-angle in range [0, 262143)
    ///                 radius = discrete pseudo-distance in range [0, 262143)
    /// From this we construct the "real" coordinates using some pseudo-randomness
    math::rng::Mersenne coordsDice{dice, coordsMask};

    // Real angle is between phi and phi + 1, then converted to radians
    const auto angle = qty::BoundedRadian::fromModulo(
        rangeRadian.high() * (static_cast<double>(rawAngle) + coordsDice.uniform(range01)) / identifierMax,
        rangeRadian);
    // We simulate a spiral galaxy with bulge, which is close to a guassian shape with most stars close to center
    const double dist = 1.000005 - (static_cast<double>(rawRadius) + coordsDice.uniform(range01)) / identifierMax;
    // -> Radius of the galaxy is comprised in range [4e-5, 140] kly
    const KiloLightYear galactic_radius{40. * std::sqrt(std::max(1e-12, -std::log(dist)))};
    // -> Height depends on distance with diameter ~ 100x thickness: [-1, 1] kly at center, [-0.1, 0.1] kly far away
    const KiloLightYear galactic_height{coordsDice.uniform(rangePM1) * (0.9 * std::pow(std::min(1., dist), 3.) + 0.1)};

    return {angle, galactic_radius.base(), galactic_height.base()};
}

} // namespace

SystemIdentifier::SystemIdentifier(uint32_t angle, uint32_t radius)
    : angle_{angle}
    , radius_{radius}
    , dice_{makeSystemDice(angle_, radius_)}
    , coords_{makeCoordinates(dice_, angle_, radius_)}
{
    if ((angle_ & identifierMask) != angle_)
    {
        throw std::runtime_error("Bad identifier angle");
    }

    if ((radius_ & identifierMask) != radius_)
    {
        throw std::runtime_error("Bad identifier radius");
    }
}

SystemIdentifier SystemIdentifier::fromValue(uint64_t value)
{
    return SystemIdentifier{static_cast<uint32_t>(value & identifierMask),
                            static_cast<uint32_t>((value >> identifierShift) & identifierMask)};
}

uint64_t SystemIdentifier::asValue() const { return valueFrom(radius_, angle_); }

const orbit::coordinates::GalactoCentric& SystemIdentifier::coordinates() const { return coords_; }

} // namespace system
} // namespace galaxias
