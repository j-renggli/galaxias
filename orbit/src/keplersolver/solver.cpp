#include "solver.h"

#include "degenerate.h"
#include "elliptic.h"
#include "hyperbolic.h"
#include "parabolic.h"
#include <math/analytic_roots.h>
#include <math/solver/brent.h>
#include <math/solver/newton_raphson.h>

namespace galaxias
{
namespace orbit
{

UniversalKeplerSolver::UniversalKeplerSolver(const CenterOfMass& com)
    : com_{com}
    , r0_{com.initialPosition().norm().value()}
    , rdotv_{com.initialPosition().dot(com.initialVelocity()).value()}
    , k_{com.mu().value()}
    , beta_{k_ * com.orbitalElements().alpha_.value()}
    , sb_{sqrt(std::abs(beta_))}
    , t0_{com.initialTime().value()}
    , h_{std::numeric_limits<double>::quiet_NaN()}
{
}

std::unique_ptr<UniversalKeplerSolver> UniversalKeplerSolver::create(const CenterOfMass& com)
{
    switch (com.orbitType())
    {
    case CenterOfMass::OrbitType::Circular:
    case CenterOfMass::OrbitType::Elliptic:
        return std::make_unique<EllipticKeplerSolver>(com);
    case CenterOfMass::OrbitType::Parabolic:
        return std::make_unique<ParabolicKeplerSolver>(com);
    case CenterOfMass::OrbitType::Hyperbolic:
        return std::make_unique<HyperbolicKeplerSolver>(com);
    case CenterOfMass::OrbitType::Degenerate:
    {
        if (com.initialPosition().squaredNorm().value() + com.initialVelocity().squaredNorm().value() == 0.)
        {
            return std::make_unique<ZeroKeplerSolver>(com);
        }

        // TODO: Implement!
        throw std::logic_error("Degenerate case not solvable yet");
    }
    };
}

void UniversalKeplerSolver::setTargetTime(const qty::Second& targetTime) { h_ = targetTime.value() - t0_; }

double UniversalKeplerSolver::solveForInternal(const qty::Second& targetTime)
{
    setTargetTime(targetTime);
    guess_ = math::firstRealCubicRoot((k_ - beta_ * r0_) / 6., rdotv_ * 0.5, r0_, -h_);

    try
    {
        return math::solver::NewtonRaphson::findRoot(*this, guess_, 1e-9 * std::abs(guess_));
    }
    catch (const math::solver::ConvergenceException&)
    {
        return math::solver::Brent::findRoot(*this, bisectionRange());
    }
}

coordinates::Cartesian UniversalKeplerSolver::coordinatesAt(const qty::Second& targetTime)
{
    root_ = solveForInternal(targetTime);
    const Factors factors = factorsAt(root_);

    return coordinates::Cartesian{com_.initialPosition() * factors.f + com_.initialVelocity() * qty::Second{factors.g},
                                  com_.initialPosition() * qty::Frequency{factors.df} +
                                      com_.initialVelocity() * factors.dg};
}

} // namespace orbit
} // namespace galaxias
