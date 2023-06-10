#pragma once

#include <ratio>

namespace galaxias
{
namespace math
{
namespace unit
{

namespace ratio
{
using Zero = std::ratio<0>;
using One = std::ratio<1>;
using NegOne = std::ratio<-1>;
using Two = std::ratio<2>;
using NegTwo = std::ratio<-2>;
using Three = std::ratio<3>;
using NegThree = std::ratio<-3>;
} // namespace ratio

template <class T = ratio::Zero,
          class L = ratio::Zero,
          class M = ratio::Zero,
          class E = ratio::Zero,
          class K = ratio::Zero,
          class A = ratio::Zero,
          class I = ratio::Zero,
          class U = ratio::Zero>
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

template <class U, class U2>
struct MultiplyUnit
{
    using value_type = Unit<std::ratio_add<typename U::Time, typename U2::Time>,
                            std::ratio_add<typename U::Length, typename U2::Length>,
                            std::ratio_add<typename U::Mass, typename U2::Mass>,
                            std::ratio_add<typename U::ElectricCurrent, typename U2::ElectricCurrent>,
                            std::ratio_add<typename U::Temperature, typename U2::Temperature>,
                            std::ratio_add<typename U::Amount, typename U2::Amount>,
                            std::ratio_add<typename U::LuminousIntensity, typename U2::LuminousIntensity>,
                            std::ratio_add<typename U::Unitless, typename U2::Unitless>>;
};

template <class U, class Ratio>
struct PowerUnit
{
    using value_type = Unit<std::ratio_multiply<typename U::Time, Ratio>,
                            std::ratio_multiply<typename U::Length, Ratio>,
                            std::ratio_multiply<typename U::Mass, Ratio>,
                            std::ratio_multiply<typename U::ElectricCurrent, Ratio>,
                            std::ratio_multiply<typename U::Temperature, Ratio>,
                            std::ratio_multiply<typename U::Amount, Ratio>,
                            std::ratio_multiply<typename U::LuminousIntensity, Ratio>>;
};

template <class U, class U2>
struct DivideUnit
{
    static_assert(U::Time::den == U2::Time::den, "Time denominator must match");
    static_assert(U::Length::den == U2::Length::den, "Length denominator must match");
    static_assert(U::Mass::den == U2::Mass::den, "Mass denominator must match");
    static_assert(U::ElectricCurrent::den == U2::ElectricCurrent::den, "Electric current denominator must match");
    static_assert(U::Temperature::den == U2::Temperature::den, "Temperature denominator must match");
    static_assert(U::Amount::den == U2::Amount::den, "Amount denominator must match");
    static_assert(U::LuminousIntensity::den == U2::LuminousIntensity::den, "Luminous intensity denominator must match");
    static_assert(U::Unitless::den == U2::Unitless::den, "Unitless denominator must match");

    using value_type =
        Unit<std::ratio<U::Time::num - U2::Time::num, U::Time::den>,
             std::ratio<U::Length::num - U2::Length::num, U::Length::den>,
             std::ratio<U::Mass::num - U2::Mass::num, U::Mass::den>,
             std::ratio<U::ElectricCurrent::num - U2::ElectricCurrent::num, U::ElectricCurrent::den>,
             std::ratio<U::Temperature::num - U2::Temperature::num, U::Temperature::den>,
             std::ratio<U::Amount::num - U2::Amount::num, U::Amount::den>,
             std::ratio<U::LuminousIntensity::num - U2::LuminousIntensity::num, U::LuminousIntensity::den>,
             std::ratio<U::Unitless::num - U2::Unitless::num, U::Unitless::den>>;
};

using Unitless = Unit<>;

// Time
using Second = Unit<ratio::One>;
using SecondSquared = Unit<ratio::Two>;
using Frequency = Unit<ratio::NegOne>;
using FrequencySquared = Unit<ratio::NegTwo>;

// Length
using Metre = Unit<ratio::Zero, ratio::One>;
using PerMetre = Unit<ratio::Zero, ratio::NegOne>;
using MetreSquared = Unit<ratio::Zero, ratio::Two>;

// Mass
using Kilogram = Unit<ratio::Zero, ratio::Zero, ratio::One>;

// Temperature
using Kelvin = Unit<ratio::Zero, ratio::Zero, ratio::Zero, ratio::Zero, ratio::One>;

// Composite
using Velocity = Unit<ratio::NegOne, ratio::One>;
using Watt = Unit<ratio::NegThree, ratio::Two, ratio::One>;

} // namespace unit
} // namespace math
} // namespace galaxias
