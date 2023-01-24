#pragma once

#include "unit.h"

namespace math
{

template <class U> struct Quantity
{
    using Unit = U;

    constexpr Quantity(double value) : val_{value} {}

    template <class U2,
              class Out = typename unit::MultiplyUnit<U, U2>::value_type>
    Quantity<Out> operator*(const Quantity<U2>& rhs) const
    {
        return Quantity<Out>(val_ * rhs.value());
    }

    template <class U2,
              class Out = typename unit::DivideUnit<U, U2>::value_type>
    Quantity<Out> operator/(const Quantity<U2>& rhs) const
    {
        return Quantity<Out>(val_ / rhs.value());
    }

    double value() const { return val_; }

private:
    double val_;
};

// Unitless
using Unitless = Quantity<unit::Unitless>;

// Time
using Second = Quantity<unit::Second>;

// Length
using Metre = Quantity<unit::Metre>;

} // namespace math
