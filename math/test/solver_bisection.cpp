#include <math/solver/bisection.h>

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace math;
using namespace solver;

namespace
{

class Linear : public IFunction
{
    virtual double f(double x) const override { return x; }
    virtual double df(double) const override { return 1.; }
};

class Quadratic : public IFunction
{
    virtual double f(double x) const override { return x * x - 3. * x - 6; }
    virtual double df(double x) const override { return 2. * x - 3.; }
};

} // namespace

TEST_CASE("Bisection with y = x")
{
    Linear lin;
    Bisection bis;

    CHECK_THROWS_AS(bis.findRoot(lin, Range<double>(1., 2.)),
                    std::runtime_error);
    CHECK_THROWS_AS(bis.findRoot(lin, Range<double>(-2., -1.)),
                    std::runtime_error);

    CHECK(bis.findRoot(lin, Range<double>(0., 1.5)) == 0.);
}

TEST_CASE("Bisection with y = x*x -3x -6")
{
    Quadratic lin;
    Bisection bis;

    SECTION("left root")
    {
        CHECK(bis.findRoot(lin, Range<double>(-2., 3.)) ==
              Approx(-1.3722813204));
    }

    SECTION("right root")
    {
        CHECK(bis.findRoot(lin, Range<double>(0., 5.)) == Approx(4.3722813204));
    }
}
