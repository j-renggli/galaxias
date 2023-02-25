#include "../src/keplersolver/hyperbolic.h"

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
const CenterOfMass com(mu, time0, Cartesian{{{1e9, 0., 0.}}, {{0., 1e6, 0.}}}, nullptr);
} // namespace

TEST_CASE("Hyperbolic construction")
{
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Hyperbolic);

    auto solver = UniversalKeplerSolver::create(com);
    REQUIRE(dynamic_cast<HyperbolicKeplerSolver*>(solver.get()) != nullptr);

    const double root = solver->solveForInternal(time1);
    const double guess = solver->initialGuess();
    CHECK(guess == Approx(0.0000016096));
    CHECK(solver->f(guess) == Approx(400.7671433214));
    CHECK(solver->df(guess) == Approx(1800030846.725025177));

    CHECK(1e6 * root == Approx(-3.9959434616));
    CHECK(solver->f(root) == Approx(-4581.6122581557));
    CHECK(solver->df(root) == Approx(-52377053818.5091247559));

    const auto factors = solver->factorsAt(guess);
    CHECK(factors.f == Approx(0.9999993621));
    CHECK(factors.g == Approx(4000.7655449367));
    CHECK(1e6 * factors.df == Approx(-0.0005315463));
    CHECK(factors.dg == Approx(0.9999998724));

    const auto rootFactors = solver->factorsAt(root);
    CHECK(rootFactors.f == Approx(0.9999994355));
    CHECK(rootFactors.g == Approx(-981.5998316672));
    CHECK(1e6 * rootFactors.df == Approx(-0.0002068405));
    CHECK(rootFactors.dg == Approx(0.9999998777));
}

TEST_CASE("Hyperbolic orbit")
{
    // Prepare solutions at a few selected times
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Hyperbolic);

    const std::vector<double> t = {
        -300.,
        -250.,
        -200.,
        -150.,
        -100.,
        -50.,
        0.,
        50.,
        100.,
        150.,
        200.,
        250.,
    };
    const std::vector<double> guess = {
        -3.55786e-07,
        -2.87324e-07,
        -2.2302e-07,
        -1.62486e-07,
        -1.05355e-07,
        -5.1293e-08,
        1.48231e-21,
        4.87904e-08,
        9.53134e-08,
        1.39776e-07,
        1.82361e-07,
        2.2323e-07,

    };
    const std::vector<double> s = {
        -3.56873e-07,
        -2.87726e-07,
        -2.23152e-07,
        -1.6252e-07,
        -1.0536e-07,
        -5.12933e-08,
        -1.1817e-27,
        4.87901e-08,
        9.53101e-08,
        1.39762e-07,
        1.8232e-07,
        2.2314e-07,
    };
    const std::vector<double> x = {
        -25.6243,
        -16.6083,
        -9.96501,
        -5.27559,
        -2.21445,
        -0.524474,
        0,
        -0.474524,
        -1.81182,
        -3.89935,
        -6.64334,
        -9.96501,
    };
    const std::vector<double> y = {
        -3.00138e+08,
        -2.50033e+08,
        -2.00006e+08,
        -1.50001e+08,
        -1e+08,
        -5e+07,
        -1.1817e-12,
        5e+07,
        9.99999e+07,
        1.5e+08,
        1.99998e+08,
        2.49995e+08,
    };
    const std::vector<double> vx = {
        0.25438,
        0.174431,
        0.119586,
        0.0790088,
        0.0473338,
        0.0215908,
        4.71025e-22,
        -0.0185711,
        -0.0348775,
        -0.0494417,
        -0.0626368,
        -0.0747365,
    };
    const std::vector<double> vy = {
        -0.0366544,
        -0.0221523,
        -0.0124573,
        -0.00620666,
        -0.0024605,
        -0.000552077,
        0.,
        -0.000451927,
        -0.00164711,
        -0.00339073,
        -0.00553605,
        -0.00797177,
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
        CHECK(coords.position()[0].value() == Approx(x[i] + com.initialPosition()[0].value()));
        CHECK(coords.position()[1].value() == Approx(y[i]));
        CHECK(coords.position()[2].value() == 0.);
        CHECK(coords.velocity()[0].value() == Approx(vx[i]));
        CHECK(coords.velocity()[1].value() == Approx(vy[i] + com.initialVelocity()[1].value()));
        CHECK(coords.velocity()[2].value() == 0.);

        // Ensure conservation of energy
        CHECK(coords.position().cross(coords.velocity())[2].value() == Approx(h0));
    }
}
