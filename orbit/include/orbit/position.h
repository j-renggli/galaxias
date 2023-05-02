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

    friend std::ostream& operator<<(std::ostream& out, const Cartesian& cartesian);

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

class GalactoCentric
{
public:
    GalactoCentric(const math::quantity::Radian& angle,
                   const math::quantity::Metre& radius,
                   const math::quantity::Metre& height);

    const math::quantity::Radian& angle() const { return angle_; }
    const math::quantity::Metre& radius() const { return radius_; }
    const math::quantity::Metre& height() const { return height_; }

    friend std::ostream& operator<<(std::ostream& out, const GalactoCentric& cartesian);

private:
    const math::quantity::Radian angle_;
    const math::quantity::Metre radius_;
    const math::quantity::Metre height_;
};

} // namespace coordinates
} // namespace orbit
} // namespace galaxias
