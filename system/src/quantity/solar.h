#pragma once

#include <math/bounded_quantity.h>
#include <math/derived_quantity.h>

namespace galaxias
{
namespace system
{

namespace qty = math::quantity;
namespace ratio = math::unit::ratio;

namespace detail
{
using Magnitude =
    math::unit::Unit<ratio::Zero, ratio::Zero, ratio::Zero, ratio::Zero, ratio::Zero, ratio::Zero, ratio::One>;
} // namespace detail

class SolarRadius : public qty::DerivedQuantity<qty::BoundedMetre>
{
public:
    static constexpr double factor{6.96342e8};
    SolarRadius(double x)
        : DerivedQuantity(x, factor, math::Range<double>::positive())
    {
    }
};

class SolarMass : public qty::DerivedQuantity<qty::BoundedKilogram>
{
public:
    static constexpr double factor{1.9885e30};
    SolarMass(double x)
        : DerivedQuantity(x, factor, math::Range<double>::positive())
    {
    }
};

class SolarLuminosity : public qty::DerivedQuantity<qty::BoundedWatt>
{
public:
    static constexpr double factor{3.828e26};
    SolarLuminosity(double x)
        : DerivedQuantity(x, factor, math::Range<double>::positive())
    {
    }
};

using BolometricMagnitude = qty::Quantity<double, detail::Magnitude>;
using ApparentMagnitude = qty::Quantity<double, detail::Magnitude>;

} // namespace system
} // namespace galaxias
