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
