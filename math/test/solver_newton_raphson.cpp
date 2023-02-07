#include <math/solver/newton_raphson.h>

#include "utils/functions.h"

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace math;
using namespace solver;
using namespace test;

TEST_CASE("Newton Raphson with y = x")
{
    Linear lin;
    NewtonRaphson nr;

    CHECK(nr.findRoot(lin, Range<double>(1., 2.)) == 0.);
    CHECK(nr.findRoot(lin, Range<double>(-2., -1.)) == 0.);

    CHECK(nr.findRoot(lin, Range<double>(0., 1.5)) == 0.);
}

TEST_CASE("Newton Raphson with y = x*x -3x -6")
{
    Quadratic lin;
    NewtonRaphson nr;

    SECTION("left root") { CHECK(nr.findRoot(lin, Range<double>(-2., 3.)) == Approx(-1.3722813204)); }

    SECTION("right root") { CHECK(nr.findRoot(lin, Range<double>(0., 5.)) == Approx(4.3722813204)); }
}
