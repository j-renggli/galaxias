#pragma once

#include "unit.h"

#include <cmath>

namespace galaxias
{
namespace math
{
namespace quantity
{

template <class T, class U>
struct Quantity
{
    using value_type = T;
    using Unit = U;

    constexpr Quantity(const T& value)
        : val_{value}
    {
    }
    constexpr Quantity(const Quantity&) = default;
    Quantity(Quantity&& rhs) noexcept = default;
    ~Quantity() = default;

    Quantity& operator=(const Quantity&) = default;
    Quantity& operator=(Quantity&&) noexcept = default;

    /// Comparison operators
    bool operator<(const Quantity& rhs) const { return val_ < rhs.val_; }
    bool operator<(const T& value) const { return val_ < value; }
    bool operator<=(const Quantity& rhs) const { return val_ <= rhs.val_; }
    bool operator<=(const T& value) const { return val_ <= value; }
    bool operator==(const Quantity& rhs) const { return val_ == rhs.val_; }
    bool operator==(const T& value) const { return val_ == value; }
    bool operator>=(const Quantity& rhs) const { return val_ >= rhs.val_; }
    bool operator>=(const T& value) const { return val_ >= value; }
    bool operator>(const Quantity& rhs) const { return val_ > rhs.val_; }
    bool operator>(const T& value) const { return val_ > value; }

    /// Unary minus
    Quantity operator-() const { return Quantity(-val_); }

    /// Add another quantity
    Quantity operator+(const Quantity& rhs) const { return Quantity(val_ + rhs.value()); }
    Quantity operator-(const Quantity& rhs) const { return Quantity(val_ - rhs.value()); }
    Quantity operator*(double scalar) const { return Quantity(val_ * scalar); }

    /// Multiply with another quantity
    template <class T2, class U2, class Out = typename unit::MultiplyUnit<U, U2>::value_type>
    Quantity<T, Out> operator*(const Quantity<T2, U2>& rhs) const
    {
        return Quantity<T, Out>(val_ * rhs.value());
    }

    /// Divide by another quantity
    template <class T2, class U2, class Out = typename unit::DivideUnit<U, U2>::value_type>
    Quantity<T, Out> operator/(const Quantity<T2, U2>& rhs) const
    {
        return Quantity<T, Out>(val_ / rhs.value());
    }

    // Useful functions

    /// Elevate to the nth power
    template <int N, int D = 1, class Out = typename unit::PowerUnit<U, std::ratio<N, D>>::value_type>
    Quantity<T, Out> pow() const
    {
        return Quantity<T, Out>(std::pow(val_, static_cast<double>(N) / static_cast<double>(D)));
    }

    /// Take the positive result of the nth root (helper function)
    template <int N, class Out = typename unit::PowerUnit<U, std::ratio<1, N>>::value_type>
    Quantity<T, Out> root() const
    {
        return pow<1, N>();
    }

    /// Retrieve the raw value
    const T& value() const { return val_; }

    // Useful functions for vectors
    Quantity<double, U> norm() const { return Quantity<double, U>{val_.norm()}; }
    Quantity<double, typename unit::MultiplyUnit<U, U>::value_type> squaredNorm() const
    {
        return Quantity<double, typename unit::MultiplyUnit<U, U>::value_type>{val_.squaredNorm()};
    }
    template <class U2, class Out = typename unit::MultiplyUnit<U, U2>::value_type>
    Quantity<double, Out> dot(const Quantity<T, U2>& rhs) const
    {
        return Quantity<double, Out>{val_.dot(rhs.value())};
    }
    template <class U2, class Out = typename unit::MultiplyUnit<U, U2>::value_type>
    Quantity<double, Out> cross(const Quantity<T, U2>& rhs) const
    {
        return Quantity<T, Out>{val_.cross(rhs.value())};
    }
    Quantity<double, U> operator[](int index) const { return Quantity<double, U>{val_[index]}; }

private:
    T val_;
};

// Unitless
using Unitless = Quantity<double, unit::Unitless>;
using Radian = Quantity<double, unit::Unitless>;

// Time
using Second = Quantity<double, unit::Second>;
using Frequency = Quantity<double, unit::Frequency>;

// Length
using Metre = Quantity<double, unit::Metre>;
using PerMetre = Quantity<double, unit::PerMetre>;

// Mass
using Kilogram = Quantity<double, unit::Kilogram>;

// Composite
using Velocity = Quantity<double, unit::Velocity>;

} // namespace quantity
} // namespace math
} // namespace galaxias
