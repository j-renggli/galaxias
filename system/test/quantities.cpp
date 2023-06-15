#include "../src/quantity/earth.h"
#include "../src/quantity/galactic.h"
#include "../src/quantity/solar.h"

#include <catch2/catch.hpp>

using namespace galaxias;
using namespace system;

template <class T, class B>
void checkDerivedUnit(const std::string& context, const double factor)
{
    INFO(context);
    const T qty{1.};
    CHECK(qty.value() == 1.);
    CHECK(qty.base().value() == factor);
    const T qtyNeg{-1.};
    CHECK(qtyNeg.value() == -1.);
    CHECK(qtyNeg.base().value() == -factor);
    //    static_assert(typename B::Unit == typename T::Base::Unit, "Wrong underlying type");
    CHECK(B{qty.base()}.value() == factor);
}

template <class T, class B>
void checkPositiveDerivedUnit(const std::string& context, const double factor)
{
    INFO(context);
    const T qty{1.};
    CHECK(qty.value() == 1.);
    CHECK(qty.base().value() == factor);
    CHECK_THROWS_AS(T{-1.}, std::runtime_error);
    //    static_assert(typename B::Unit == typename T::Base::Unit, "Wrong underlying type");
    CHECK(B{qty.base()}.value() == factor);
}

TEST_CASE("Galactic quantities")
{
    checkDerivedUnit<Parsec, math::quantity::Metre>("Parsec", 3.0857e16);
    checkDerivedUnit<LightYear, math::quantity::Metre>("LightYear", 9.4607e15);
    checkDerivedUnit<KiloLightYear, math::quantity::Metre>("KiloLightYear", 9.46073047258e18);

    // Check some conversions
    const Parsec pc{1};
    const LightYear ly{1};
    const KiloLightYear kly{1};
    CHECK(pc.base().value() == Approx(ly.base().value() * 3.26156));
    CHECK(kly.base().value() == Approx(ly.base().value() * 1000.));
    CHECK(ly.base().value() == Approx(AstronomicalUnit{1}.base().value() * 63241.));
}

TEST_CASE("Solar quantities")
{
    checkPositiveDerivedUnit<SolarMass, math::quantity::Kilogram>("SolarMass", 1.9885e30);
    checkPositiveDerivedUnit<SolarRadius, math::quantity::Metre>("SolarRadius", 696342000.0);
}

TEST_CASE("Earth quantities")
{
    checkPositiveDerivedUnit<EarthMass, math::quantity::Kilogram>("EarthMass", 5.9722e24);
    checkPositiveDerivedUnit<EarthRadius, math::quantity::Metre>("EarthRadius", 6378100);
    checkPositiveDerivedUnit<AstronomicalUnit, math::quantity::Metre>("AstronomicalUnit", 1.495978707e11);
}
