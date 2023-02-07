#pragma once

#include "quantity.h"

#include <stdexcept>

namespace galaxias
{
namespace math
{
namespace quantity
{

template <class U>
struct BoundedQuantity : public Quantity<U>
{
    constexpr BoundedQuantity(const Quantity<U>& qty,
                              double min = -std::numeric_limits<double>::max(),
                              double max = std::numeric_limits<double>::max())
        : Quantity<U>{qty}
        , min_{min}
        , max_{max}
    {
        if (this->value() < min_ || this->value() > max_)
        {
            throw std::runtime_error("Out of bounds " + std::to_string(min_) + " <= " + std::to_string(this->value()) +
                                     " <= " + std::to_string(max_));
        }
    }

    double min() const { return min_; }
    double max() const { return max_; }

private:
    double min_;
    double max_;
};

// Unitless
using BoundedUnitless = BoundedQuantity<unit::Unitless>;
using BoundedRadian = BoundedQuantity<unit::Unitless>;

// Time
using BoundedSecond = BoundedQuantity<unit::Second>;
using BoundedFrequency = BoundedQuantity<unit::Frequency>;

// Length
using BoundedMetre = BoundedQuantity<unit::Metre>;

// Composite
using BoundedVelocity = BoundedQuantity<unit::Velocity>;

} // namespace quantity
} // namespace math
} // namespace galaxias
