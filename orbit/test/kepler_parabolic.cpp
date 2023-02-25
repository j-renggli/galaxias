#include "../src/keplersolver/parabolic.h"

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
const CenterOfMass com(mu, time0, Cartesian{{{2. * mu.value() / 1e6, 0., 0.}}, {{0., 1e3, 0.}}}, nullptr);
} // namespace

TEST_CASE("Parabolic construction")
{
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Parabolic);

    auto solver = UniversalKeplerSolver::create(com);
    REQUIRE(dynamic_cast<ParabolicKeplerSolver*>(solver.get()) != nullptr);

    const double root = solver->solveForInternal(time1);
    const double guess = solver->initialGuess();
    CHECK(guess == Approx(0.0000045056));
    CHECK(root == Approx(guess));
    CHECK(1e6 * solver->f(guess) == Approx(0.0003938112));
    CHECK(solver->df(guess) == Approx(800796831.5670608282));

    CHECK(/*1e13 **/ solver->f(root) == Approx(/*1e13 **/ -4.5474735089e-13));
    CHECK(solver->df(root) == Approx(800796831.5670604706));

    const auto factors = solver->factorsAt(guess);
    CHECK(factors.f == Approx(0.9999949248));
    CHECK(factors.g == Approx(3599.9939234559));
    CHECK(1e6 * factors.df == Approx(-0.0028132243));
    CHECK(factors.dg == Approx(0.9999949476));

    const auto rootFactors = solver->factorsAt(root);
    CHECK(rootFactors.f == Approx(0.9999949248));
    CHECK(rootFactors.g == Approx(3599.9939234559));
    CHECK(1e6 * rootFactors.df == Approx(-0.0028132243));
    CHECK(rootFactors.dg == Approx(0.9999949476));
}

TEST_CASE("Parabolic orbit")
{
    // Prepare solutions at a few selected times
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Parabolic);
    const double x0 = com.initialPosition()[0].value();

    const std::vector<double> t = {
        -2400.,
        -2000.,
        -1600.,
        -1200.,
        -800.,
        -400.,
        0.,
        400.,
        800.,
        1200.,
        1600.,
        2000.,
    };
    const std::vector<double> guess = {
        -3.01508e-06,
        -2.51193e-06,
        -2.00904e-06,
        -1.5064e-06,
        -1.00402e-06,
        -5.01882e-07,
        4.33681e-19,
        5.0163e-07,
        1.00301e-06,
        1.50414e-06,
        2.00501e-06,
        2.50564e-06,

    };
    const std::vector<double> s = guess;
    const std::vector<double> x = {
        -1811.78,
        -1257.54,
        -804.424,
        -452.261,
        -200.904,
        -50.2007,
        0.,
        -50.1504,
        -200.501,
        -450.901,
        -801.201,
        -1251.25,
    };
    const std::vector<double> y = {
        -2.4e+06,
        -2e+06,
        -1.6e+06,
        -1.2e+06,
        -800000,
        -400000,
        1.15151e-22,
        400000,
        800000,
        1.2e+06,
        1.6e+06,
        2e+06,
    };
    const std::vector<double> vx = {
        1.51209,
        1.25913,
        1.00654,
        0.754336,
        0.502512,
        0.251067,
        -7.22223729145e-29,
        -0.250689,
        -0.501001,
        -0.750938,
        -1.0005,
        -1.24969,
    };
    const std::vector<double> vy = {
        999.998,
        999.998,
        999.999,
        999.999,
        1000,
        1000,
        1000,
        1000,
        999.999,
        999.999,
        999.998,
        1000,
    };

    auto solver = UniversalKeplerSolver::create(com);

    const auto c0 = com.initialCoordinates();
    const double h0 = c0.position().cross(c0.velocity())[2].value();

    for (size_t i = 0; i < t.size(); ++i)
    {
        INFO(i << ": " << t[i]);
        if (t[i] == 0.)
        {
            CHECK(std::abs(solver->solveForInternal(t[i])) < 1e-30);
        }
        else
        {
            CHECK(solver->solveForInternal(t[i]) == Approx(s[i]));
        }
        CHECK(solver->initialGuess() == Approx(guess[i]));
        const auto coords = solver->coordinatesAt(t[i]);
        CHECK(coords.position()[0].value() == Approx(x0 + x[i]));
        CHECK(coords.position()[1].value() == Approx(y[i]));
        CHECK(coords.position()[2].value() == 0.);
        CHECK(coords.velocity()[0].value() == Approx(vx[i]));
        CHECK(coords.velocity()[1].value() == Approx(vy[i]));
        CHECK(coords.velocity()[2].value() == 0.);

        // Ensure conservation of energy
        CHECK(coords.position().cross(coords.velocity())[2].value() == Approx(h0));
    }
}
