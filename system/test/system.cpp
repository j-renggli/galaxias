#include <system/system.h>

#include "../src/quantity/galactic.h"

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace system;

TEST_CASE("System constructor")
{
    auto system = ISystem::create(SystemIdentifier::fromValue(2));
    CHECK(system->galacticCoords().angle().value() == Approx(0.0000566644));
    CHECK(system->galacticCoords().radius().value() / KiloLightYear::factor == Approx(0.00004));
    CHECK(system->galacticCoords().height().value() / KiloLightYear::factor == Approx(0.8131600274));
    // 1
    //    CHECK(system->galacticCoords().angle().value() == Approx(0.0000276801));
    //    CHECK(system->galacticCoords().radius().value() / KiloLightYear::factor == Approx(0.00004));
    //    CHECK(system->galacticCoords().height().value() / KiloLightYear::factor == Approx(-0.4866674453));
}
