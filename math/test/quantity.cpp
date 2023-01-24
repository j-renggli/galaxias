#include <math/quantity.h>

#include <catch2/catch.hpp>

using namespace math;
using namespace math::quantity;

namespace
{
constexpr double two{2.};
constexpr double pi{3.14};

template <class R> void checkRatio(double expected)
{
    CHECK(static_cast<double>(R::num) / static_cast<double>(R::den) ==
          expected);
}

template <class T>
void checkUnit(const T&, const std::array<double, 8>& expectedRatios)
{
    {
        INFO("Time");
        checkRatio<typename T::Unit::Time>(expectedRatios[0]);
    }
    {
        INFO("Length");
        checkRatio<typename T::Unit::Length>(expectedRatios[1]);
    }
}

} // namespace

TEST_CASE("Unitless")
{
    const Unitless x{pi};
    CHECK(x.value() == pi);
    checkUnit(x, {{0., 0., 0., 0., 0., 0., 0., 0.}});
}

TEST_CASE("Second")
{
    const Second s{pi};
    CHECK(s.value() == pi);
    checkUnit(s, {{1., 0., 0., 0., 0., 0., 0., 0.}});
}

TEST_CASE("Metre")
{
    const Metre m{pi};
    CHECK(m.value() == pi);
    checkUnit(m, {{0., 1., 0., 0., 0., 0., 0., 0.}});
}

TEST_CASE("Multiplication")
{
    {
        // Same dimension
        INFO("m*m");
        const Metre ma{two};
        const Metre mb{pi};
        const auto m2 = ma * mb;

        CHECK(m2.value() == two * pi);
        checkUnit(m2, {{0., 2., 0., 0., 0., 0., 0., 0.}});
    }

    {
        // Different dimensions
        INFO("m*s");
        const Metre m{two};
        const Second s{pi};
        const auto ms = m * s;
        const auto sm = s * m;

        CHECK(ms.value() == two * pi);
        checkUnit(ms, {{1., 1., 0., 0., 0., 0., 0., 0.}});
        CHECK(sm.value() == two * pi);
        checkUnit(sm, {{1., 1., 0., 0., 0., 0., 0., 0.}});
    }
}

TEST_CASE("Division")
{
    {
        // Same dimension
        INFO("m/m");
        const Metre ma{pi};
        const Metre mb{two};
        const auto m2 = ma / mb;

        CHECK(m2.value() == pi / two);
        checkUnit(m2, {{0., 0., 0., 0., 0., 0., 0., 0.}});
    }

    {
        // Different dimensions
        const Metre m{two};
        const Second s{pi};
        const auto ms = m / s;
        const auto sm = s / m;

        {
            INFO("m/s");
            CHECK(ms.value() == two / pi);
            checkUnit(ms, {{-1., 1., 0., 0., 0., 0., 0., 0.}});
        }
        {
            INFO("s/m");
            CHECK(sm.value() == pi / two);
            checkUnit(sm, {{1., -1., 0., 0., 0., 0., 0., 0.}});
        }
    }
}

TEST_CASE("Composite")
{
    {
        INFO("m/s");
        const Velocity v{two};
        CHECK(v.value() == two);
        checkUnit(v, {{-1., 1., 0., 0., 0., 0., 0., 0.}});
    }

    {
        INFO("m*m*m/s*s");
        auto mu = Metre{2.} * Metre{3.} * Metre{4.} / (Second{6.} * Second{2.});
        CHECK(mu.value() == 2.);
        checkUnit(mu, {{-2., 3., 0., 0., 0., 0., 0., 0.}});
    }
}
