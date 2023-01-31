#pragma once

#include <math/quantity.h>

#include <array>

namespace galaxias
{
namespace orbit
{
namespace coordinates
{

class Cartesian
{
public:
    using Position = std::array<math::quantity::Metre, 3>;
    using Velocity = std::array<math::quantity::Velocity, 3>;

    Cartesian(const Position& position, const Velocity& velocity);

    const Position& position() const { return position_; }
    const Velocity& velocity() const { return velocity_; }

private:
    Position position_;
    Velocity velocity_;
};

} // namespace coordinates
} // namespace orbit
} // namespace galaxias
