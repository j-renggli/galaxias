#include <system/system.h>

#include "../src/galactic_quantity.h"

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace system;

namespace
{

constexpr double idLimit{262144.};
constexpr double angularLimit{2. * M_PI / idLimit};

double radius(double r)
{
    const double dist = 1.000005 - r / idLimit;
    return 40. * std::sqrt(std::max(1e-12, -std::log(dist)));
}

double height(double r)
{
    const double dist = 1.000005 - r / idLimit;
    return 0.9 * std::pow(std::min(1., dist), 3.) + 0.1;
}

} // namespace

TEST_CASE("System identifier from valid separate values")
{
    for (uint32_t a = 0; a < 0x40000; a += 0x8642)
    {
        for (uint32_t r = 0; r < 0x40000; r += 0x7531)
        {
            SystemIdentifier si(a, r);
            CHECK(si.angle() == a);
            CHECK(si.radius() == r);

            // And galactic coordinates are within expected limits
            const auto coords = si.coordinates();
            CHECK(coords.angle().value() >= static_cast<double>(a) * angularLimit);
            CHECK(coords.angle().value() <= static_cast<double>(a + 1) * angularLimit);

            const double rMin = radius(static_cast<double>(r));
            const double rMax = radius(static_cast<double>(r + 1));
            const double hMax = height(static_cast<double>(r));

            CHECK(coords.height().value() >= -hMax * KiloLightYear::factor);
            CHECK(coords.height().value() <= hMax * KiloLightYear::factor);
            CHECK(coords.radius().value() >= rMin * KiloLightYear::factor);
            CHECK(coords.radius().value() <= rMax * KiloLightYear::factor);
        }
    }
}

TEST_CASE("System identifier from invalid separate values")
{
    CHECK_THROWS(SystemIdentifier(0x40000, 1));
    CHECK_THROWS(SystemIdentifier(1, 0x40000));
}

TEST_CASE("System identifier from value")
{
    for (uint64_t i = 0; i < (uint64_t(1) << 36); i += 0x654321)
    {
        auto x = SystemIdentifier::fromValue(i);
        CHECK(x.asValue() == i);
    }
}
