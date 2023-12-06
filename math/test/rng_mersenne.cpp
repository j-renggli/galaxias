#include <math/rng/prng.h>

#include <math/range.h>

#include <catch2/catch.hpp>

#include <deque>

using namespace galaxias;
using namespace math;
using namespace rng;

namespace
{
constexpr Range<double> range(10., 20.);
constexpr Range<size_t> intRange(15, 30);

} // namespace

TEST_CASE("Mersenne with arbitrary seed")
{
    Mersenne m{};

    const double x = m.uniform(range);
    CHECK(x >= 10.);
    CHECK(x <= 20.);
}

TEST_CASE("Mersenne with seed")
{
    Mersenne m{42};

    // m produces deterministic outputs
    CHECK(m.uniform(range) == Approx(17.5515553295));
    CHECK(m.uniform() == 11788048577503494824ull);
    CHECK(m.gaussian<double>() == Approx(-0.5740948067));
    CHECK(m.gaussian(10.) == Approx(8.0933146552));
    CHECK(m.gaussian(-5., 0.3) == Approx(-5.2172368796));
    CHECK(m.uniform(intRange) == 15);
}

TEST_CASE("Mersenne from existing and mask")
{
    constexpr int64_t mask = 0x7777777777777777;
    Mersenne m{42};
    Mersenne n{m, mask};

    // m has been updated in the process
    CHECK(m.uniform() == 11788048577503494824ull);
    // n produces deterministic outputs
    CHECK(n.uniform() == 15816580110690532602ull);
}
