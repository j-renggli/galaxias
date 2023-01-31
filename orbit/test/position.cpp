#include <orbit/position.h>

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace orbit;
using namespace coordinates;

namespace
{
using M = math::quantity::Metre;
using V = math::quantity::Velocity;
} // namespace

TEST_CASE("Cartesian")
{
    const Cartesian coords{{{M{1.}, M{2.}, M{3.}}}, {{V{4.}, V{5.}, V{6.}}}};
    CHECK(coords.position()[0].value() == 1.);
    CHECK(coords.position()[1].value() == 2.);
    CHECK(coords.position()[2].value() == 3.);
    CHECK(coords.velocity()[0].value() == 4.);
    CHECK(coords.velocity()[1].value() == 5.);
    CHECK(coords.velocity()[2].value() == 6.);
}
