#pragma once

#include <math/rng/prng.h>
#include <orbit/position.h>

#include <cstdint>

namespace galaxias
{
namespace system
{

class SystemIdentifier
{
public:
    SystemIdentifier(uint32_t angle, uint32_t radius);

    static SystemIdentifier fromValue(uint64_t value);
    uint64_t asValue() const;

    uint32_t angle() const { return angle_; }
    uint32_t radius() const { return radius_; }

    /// Retrieve the underlying dice for this system. Careful in order of call!
    math::rng::Random& dice() { return dice_; }
    const orbit::coordinates::GalactoCentric& coordinates() const;

private:
    uint32_t angle_;
    uint32_t radius_;

    math::rng::Random dice_;
    orbit::coordinates::GalactoCentric coords_;
};

} // namespace system
} // namespace galaxias
