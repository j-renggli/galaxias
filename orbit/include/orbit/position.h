#pragma once

#include <math/quantity.h>

#include <array>
#include <eigen3/Eigen/Dense>

namespace galaxias
{
namespace orbit
{

using Vector = Eigen::Vector3d;

namespace coordinates
{

class Cartesian
{
public:
    using Position = math::quantity::Quantity<Vector, math::unit::Metre>;
    using Velocity = math::quantity::Quantity<Vector, math::unit::Velocity>;

    Cartesian(const Position& position, const Velocity& velocity);
    static Cartesian zero();

    const Position& position() const { return r_; }
    double normPosition() const { return r_.value().norm(); }
    const Velocity& velocity() const { return v_; }
    double normVelocity() const { return v_.value().norm(); }

private:
    Position r_;
    Velocity v_;
};

class StelloCentric : public Cartesian
{
};

class Perifocal : public Cartesian
{
};

} // namespace coordinates
} // namespace orbit
} // namespace galaxias
