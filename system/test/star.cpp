#include "../src/bodies/star.h"

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace system;

TEST_CASE("Star constructor")
{
    math::rng::Random rng{1};
    Star star{std::move(rng)};
    CHECK(star.mass().value() == Approx{8.52478e29});
    CHECK(star.radius().value() == Approx{4.2184634e8});
}
