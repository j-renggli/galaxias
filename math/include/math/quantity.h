#pragma once

#include "unit.h"

#include <cmath>

namespace galaxias
{
namespace math
{
namespace quantity
{

template <class U>
struct Quantity
{
    using Unit = U;

    constexpr Quantity(double value)
        : val_{value}
    {
    }

    /// Unary minus
    Quantity operator-() const { return Quantity(-val_); }

    /// Multiply with another quantity
    template <class U2,
              class Out = typename unit::MultiplyUnit<U, U2>::value_type>
    Quantity<Out> operator*(const Quantity<U2>& rhs) const
    {
        return Quantity<Out>(val_ * rhs.value());
    }

    /// Divide by another quantity
    template <class U2,
              class Out = typename unit::DivideUnit<U, U2>::value_type>
    Quantity<Out> operator/(const Quantity<U2>& rhs) const
    {
        return Quantity<Out>(val_ / rhs.value());
    }

    /// Elevate to the nth power
    template <
        int N,
        int D = 1,
        class Out = typename unit::PowerUnit<U, std::ratio<N, D>>::value_type>
    Quantity<Out> pow() const
    {
        return Quantity<Out>(
            std::pow(val_, static_cast<double>(N) / static_cast<double>(D)));
    }

    /// Take the positive result of the nth root (helper function)
    template <
        int N,
        class Out = typename unit::PowerUnit<U, std::ratio<1, N>>::value_type>
    Quantity<Out> root() const
    {
        return pow<1, N>();
    }

    /// Retrieve the raw value
    double value() const { return val_; }

private:
    double val_;
};

// Unitless
using Unitless = Quantity<unit::Unitless>;
using Radian = Quantity<unit::Unitless>;

// Time
using Second = Quantity<unit::Second>;
using Frequency = Quantity<unit::Frequency>;

// Length
using Metre = Quantity<unit::Metre>;

// Composite
using Velocity = Quantity<unit::Velocity>;

} // namespace quantity
} // namespace math
} // namespace galaxias
