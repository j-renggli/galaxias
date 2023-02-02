#include <math/solver/brent.h>

#include "utils/functions.h"

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace math;
using namespace solver;
using namespace test;

TEST_CASE("Brent with y = x")
{
    Linear lin;
    Brent brent;

    CHECK_THROWS_AS(brent.findRoot(lin, Range<double>(1., 2.)),
                    std::runtime_error);
    CHECK_THROWS_AS(brent.findRoot(lin, Range<double>(-2., -1.)),
                    std::runtime_error);

    CHECK(brent.findRoot(lin, Range<double>(0., 1.5)) == 0.);
}

TEST_CASE("Brent with y = x*x -3x -6")
{
    Quadratic lin;
    Brent brent;

    SECTION("left root")
    {
        CHECK(brent.findRoot(lin, Range<double>(-2., 3.)) ==
              Approx(-1.3722813204));
    }

    SECTION("right root")
    {
        CHECK(brent.findRoot(lin, Range<double>(0., 5.)) ==
              Approx(4.3722813204));
    }
}
