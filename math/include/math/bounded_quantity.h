#pragma once

#include "quantity.h"
#include <math/range.h>

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
                              const Range<double>& range = Range<double>(-std::numeric_limits<double>::max(),
                                                                         std::numeric_limits<double>::max()))
        : Quantity<U>{qty}
        , range_{range}
    {
        if (!range_.includes(qty.value()))
        {
            throw std::runtime_error("Out of bounds " + std::to_string(range_.low()) +
                                     " <= " + std::to_string(this->value()) + " <= " + std::to_string(range_.high()));
        }
    }

    const Range<double>& range() const { return range_; }

private:
    Range<double> range_;
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
