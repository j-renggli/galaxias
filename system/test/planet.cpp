#include "../src/bodies/planet.h"

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace system;

TEST_CASE("Planet constructor")
{
    math::rng::Random rng{1};
    Planet star{std::move(rng)};
    CHECK(star.mass().value() == Approx{3.6768874e25});
    CHECK(star.radius().value() == Approx{2.0703877e7});
}
