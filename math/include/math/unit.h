#pragma once

#include <ratio>

namespace math
{
namespace unit
{

namespace
{
using Zero = std::ratio<0>;
using One = std::ratio<1>;
using Two = std::ratio<2>;
} // namespace

template <class T = Zero, class L = Zero, class M = Zero, class E = Zero,
          class K = Zero, class A = Zero, class I = Zero, class U = Zero>
struct Unit
{
    using Time = T;
    using Length = L;
    using Mass = M;
    using ElectricCurrent = E;
    using Temperature = K;
    using Amount = A;
    using LuminousIntensity = I;
    using Unitless = U;
};

template <class U, class U2> struct MultiplyUnit
{
    static_assert(U::Time::den == U2::Time::den, "Time denominator must match");
    static_assert(U::Length::den == U2::Length::den,
                  "Length denominator must match");
    static_assert(U::Mass::den == U2::Mass::den, "Mass denominator must match");
    static_assert(U::ElectricCurrent::den == U2::ElectricCurrent::den,
                  "Electric current denominator must match");
    static_assert(U::Temperature::den == U2::Temperature::den,
                  "Temperature denominator must match");
    static_assert(U::Amount::den == U2::Amount::den,
                  "Amount denominator must match");
    static_assert(U::LuminousIntensity::den == U2::LuminousIntensity::den,
                  "Luminous intensity denominator must match");
    static_assert(U::Unitless::den == U2::Unitless::den,
                  "Unitless denominator must match");

    using value_type = Unit<
        std::ratio<U::Time::num + U2::Time::num, U::Time::den>,
        std::ratio<U::Length::num + U2::Length::num, U::Length::den>,
        std::ratio<U::Mass::num + U2::Mass::num, U::Mass::den>,
        std::ratio<U::ElectricCurrent::num + U2::ElectricCurrent::num,
                   U::ElectricCurrent::den>,
        std::ratio<U::Temperature::num + U2::Temperature::num,
                   U::Temperature::den>,
        std::ratio<U::Amount::num + U2::Amount::num, U::Amount::den>,
        std::ratio<U::LuminousIntensity::num + U2::LuminousIntensity::num,
                   U::LuminousIntensity::den>,
        std::ratio<U::Unitless::num + U2::Unitless::num, U::Unitless::den>>;
};

// Time
using Second = Unit<One>;

// Length
using Metre = Unit<Zero, One>;
using MetreSquared = Unit<Zero, Two>;

} // namespace unit
} // namespace math
