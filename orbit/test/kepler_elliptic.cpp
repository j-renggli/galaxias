#include "../src/keplersolver/elliptic.h"

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace orbit;
using namespace coordinates;

namespace
{
const GravitationalParam mu{3.986004418e14};
constexpr double t0{0.};
constexpr qty::Second time0{t0};
constexpr qty::Second time1{t0 + 3600.};
const CenterOfMass com(mu, time0, Cartesian{{{-4500000., 4500000., 0.}}, {{0., 4000., 0.}}}, nullptr);
} // namespace

TEST_CASE("Elliptic construction")
{
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Elliptic);

    auto solver = UniversalKeplerSolver::create(com);
    REQUIRE(dynamic_cast<EllipticKeplerSolver*>(solver.get()) != nullptr);

    const double root = solver->solveForInternal(time1);
    const double guess = solver->initialGuess();
    CHECK(guess == Approx(-0.000347804));
    CHECK(solver->f(guess) == Approx(-2114.7793648556));
    CHECK(solver->df(guess) == Approx(2411429.2490112502));

    CHECK(root == Approx(0.0002264607));
    CHECK(solver->f(root) == Approx(-0.0000321557304));
    CHECK(solver->df(root) == Approx(3409186.1324010435));

    const auto factors = solver->factorsAt(guess);
    CHECK(factors.f == Approx(-0.0778838745));
    CHECK(factors.g == Approx(726.76979776));
    CHECK(factors.df == Approx(-0.0011782734));
    CHECK(factors.dg == Approx(-1.8446246041));

    const auto rootFactors = solver->factorsAt(root);
    CHECK(rootFactors.f == Approx(0.0170156354));
    CHECK(rootFactors.g == Approx(825.2248229077));
    CHECK(rootFactors.df == Approx(-0.0012290071));
    CHECK(rootFactors.dg == Approx(-0.8349465083));

    // Period results in same guess
    CHECK(com.orbitalPeriod().low() == 0.);
    CHECK(com.orbitalPeriod().high() == Approx(2192.6936220732));
    CHECK(root == Approx(solver->solveForInternal(time1 - com.orbitalPeriod().high())));
    CHECK(root == Approx(solver->solveForInternal(time1 + com.orbitalPeriod().high())));
}

TEST_CASE("Elliptic orbit")
{
    // Prepare solutions at a few selected times
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Elliptic);

    const std::vector<double> t = {
        -1140.,
        -950.,
        -760.,
        -570.,
        -380.,
        -190.,
        0.,
        190.,
        380.,
        570.,
        760.,
        950.,
    };
    const std::vector<double> guess = {
        -0.000329404,
        -0.000339537,
        -0.000349041,
        -0.000358011,
        -0.000366519,
        -0.000374622,
        -0.00025241,
        -0.000270601,
        -0.000286144,
        -0.000299875,
        -0.000312269,
        -0.000323625,

    };
    const std::vector<double> s = {
        0.000152206,
        0.000186875,
        0.000234245,
        0.000454778,
        0.000528407,
        0.000568919,
        0.,
        2.84289e-05,
        5.5086e-05,
        8.11709e-05,
        0.000107687,
        0.000135782,
    };
    const std::vector<double> x = {
        -1.86834e+06,
        -957454,
        63932.6,
        -2.02748e+06,
        -3.7908e+06,
        -4.35559e+06,
        -4.5e+06,
        -4.38694e+06,
        -4.08405e+06,
        -3.62586e+06,
        -3.03119e+06,
        -2.30969e+06,
    };
    const std::vector<double> y = {
        5.2538e+06,
        4.49638e+06,
        3.1406e+06,
        487569,
        2.3699e+06,
        3.60438e+06,
        4.5e+06,
        5.14086e+06,
        5.56036e+06,
        5.76877e+06,
        5.75975e+06,
        5.50833e+06,
    };
    const std::vector<double> vx = {
        4472.35,
        5112.16,
        5531.45,
        -20295.9,
        -4712.93,
        -1644.66,
        0.,
        1134.72,
        2024.57,
        2782.45,
        3468.98,
        4122.93,
    };
    const std::vector<double> vy = {
        -2942.1,
        -5207.76,
        -9821.93,
        13758.8,
        7694.72,
        5493.62,
        4000,
        2773.36,
        1650.97,
        537.432,
        -653.367,
        -2039.44,
    };

    auto solver = UniversalKeplerSolver::create(com);

    const auto c0 = com.initialCoordinates();
    const double h0 = c0.position().cross(c0.velocity())[2].value();

    for (size_t i = 0; i < t.size(); ++i)
    {
        INFO(i << ": " << t[i]);
        CHECK(solver->solveForInternal(t[i]) == Approx(s[i]));
        CHECK(solver->initialGuess() == Approx(guess[i]));
        const auto coords = solver->coordinatesAt(t[i]);
        CHECK(coords.position()[0].value() == Approx(x[i]));
        CHECK(coords.position()[1].value() == Approx(y[i]));
        CHECK(coords.position()[2].value() == 0.);
        CHECK(coords.velocity()[0].value() == Approx(vx[i]));
        CHECK(coords.velocity()[1].value() == Approx(vy[i]));
        CHECK(coords.velocity()[2].value() == 0.);

        // Ensure conservation of energy
        CHECK(coords.position().cross(coords.velocity())[2].value() == Approx(h0));
    }
}

TEST_CASE("Circular construction")
{
    constexpr double mu2{4e14};
    constexpr double r = 1e6;
    const CenterOfMass circular(
        GravitationalParam{mu2}, time1, Cartesian{{{0., r, 0.}}, {{sqrt(mu2 * r) / r, 0., 0.}}}, nullptr);
    CHECK(circular.orbitalElements().eccentricity_.value() == 0.);
    REQUIRE(circular.orbitType() == CenterOfMass::OrbitType::Circular);

    auto solver = UniversalKeplerSolver::create(circular);
    REQUIRE(dynamic_cast<EllipticKeplerSolver*>(solver.get()) != nullptr);

    const double root = solver->solveForInternal(time0);
    const double guess = solver->initialGuess();
    CHECK(guess == Approx(0.0000896106));
    CHECK(solver->f(guess) == Approx(-19.3200450189));
    CHECK(solver->df(guess) == Approx(1975587.5485922508));

    CHECK(root == Approx(0.0000995311));
    CHECK(solver->f(root) == Approx(-7.2475359e-12));
    CHECK(solver->df(root) == Approx(1913159.9215603475));

    const auto factors = solver->factorsAt(guess);
    CHECK(factors.f == Approx(-0.2196108718));
    CHECK(factors.g == Approx(109.7599210218));
    CHECK(factors.df == Approx(-0.0098764294));
    CHECK(factors.dg == Approx(0.3826591625));

    const auto rootFactors = solver->factorsAt(root);
    CHECK(rootFactors.f == Approx(-0.4076014692));
    CHECK(rootFactors.g == Approx(116.0380695357));
    CHECK(rootFactors.df == Approx(-0.0095460909));
    CHECK(rootFactors.dg == Approx(0.2642531064));

    // Period results in same guess
    CHECK(circular.orbitalPeriod().low() == 0.);
    CHECK(circular.orbitalPeriod().high() == Approx(314.159265359));
    CHECK(root == Approx(solver->solveForInternal(time0 - circular.orbitalPeriod().high())));
    CHECK(root == Approx(solver->solveForInternal(time0 + circular.orbitalPeriod().high())));
}
