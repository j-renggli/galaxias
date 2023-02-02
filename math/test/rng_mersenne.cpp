#include <math/rng/mersenne.h>

#include <math/range.h>

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace math;
using namespace rng;

TEST_CASE("Rng with seed")
{
    Mersenne m{42};

    CHECK(m.uniform(Range<double>(10., 20.)) == Approx(17.5515553295));
}
