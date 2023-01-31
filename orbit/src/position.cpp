#include <orbit/position.h>

namespace galaxias
{
namespace orbit
{
namespace coordinates
{

Cartesian::Cartesian(const Position& position, const Velocity& velocity)
    : position_{position}, velocity_{velocity}
{
}

} // namespace coordinates
} // namespace orbit
} // namespace galaxias
