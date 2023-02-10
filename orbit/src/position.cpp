#include <orbit/position.h>

namespace galaxias
{
namespace orbit
{
namespace coordinates
{

Cartesian::Cartesian(const Position& position, const Velocity& velocity)
    : r_{position}
    , v_{velocity}
{
}

Cartesian Cartesian::zero() { return Cartesian{Position{Position::value_type()}, Velocity{Velocity::value_type()}}; }

} // namespace coordinates
} // namespace orbit
} // namespace galaxias
