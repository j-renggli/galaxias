#include "../src/bodies/star.h"

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace system;

TEST_CASE("Star constructor")
{
    constexpr double absMag{8.4528688829};
    math::rng::Random rng{1};
    Star star{std::move(rng)};
    CHECK(star.mass().value() == Approx{8.52478e29});
    CHECK(star.radius().value() == Approx{0.6058033842});
    CHECK(star.radius().base().value() == Approx{4.2184634e8});
    CHECK(star.temperature().value() == Approx{3171.3758452529});
    CHECK(star.luminosity().value() == Approx{0.0327228045});
    CHECK(star.absoluteMagnitude().value() == Approx{absMag});
    CHECK(star.absoluteMagnitude().value() == star.apparentMagnitude(Parsec{10.}).value());
    CHECK(star.apparentMagnitude(Parsec{1.}).value() == Approx{absMag - 5.});
    CHECK(star.apparentMagnitude(Parsec{100.}).value() == Approx{absMag + 5.});
    CHECK(star.colour().hex() == "0xffbc76ff");
}
