#include <math/bounded_quantity.h>

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace math;
using namespace quantity;

namespace
{
constexpr double min{-std::numeric_limits<double>::max()};
constexpr double max{std::numeric_limits<double>::max()};
} // namespace

TEST_CASE("No bounds")
{
    const BoundedUnitless x{0.};
    CHECK(x.value() == 0.);
    CHECK(x.min() == min);
    CHECK(x.max() == max);

    CHECK_NOTHROW(BoundedUnitless(min));
    CHECK_NOTHROW(BoundedUnitless(max));
}

TEST_CASE("Minimum")
{
    const BoundedUnitless x{0., 0.};
    CHECK(x.min() == 0.);
    CHECK(x.max() == max);

    CHECK_THROWS_AS(BoundedUnitless(-1., 0.), std::runtime_error);
    CHECK_NOTHROW(BoundedUnitless(max, 0.));
}

TEST_CASE("Maximum")
{
    const BoundedUnitless x{0., min, 0.};
    CHECK(x.min() == min);
    CHECK(x.max() == 0.);

    CHECK_NOTHROW(BoundedUnitless(min, min, 0.));
    CHECK_THROWS_AS(BoundedUnitless(1., min, 0.), std::runtime_error);
}

TEST_CASE("Both")
{
    const BoundedUnitless x{0., -1., 1.};
    CHECK(x.min() == -1.);
    CHECK(x.max() == 1.);

    CHECK_THROWS_AS(BoundedUnitless(-2., -1., 1.), std::runtime_error);
    CHECK_NOTHROW(BoundedUnitless(0., -1., 1.));
    CHECK_THROWS_AS(BoundedUnitless(2., -1., 1.), std::runtime_error);

    CHECK_NOTHROW(BoundedUnitless(0., 0., 0.));
    CHECK_THROWS_AS(BoundedUnitless(0., 1., -1.), std::runtime_error);
}
