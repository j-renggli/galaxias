#pragma once

#include <math/bounded_quantity.h>
#include <math/derived_quantity.h>

namespace galaxias
{
namespace system
{

namespace qty = math::quantity;
namespace ratio = math::unit::ratio;

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

} // namespace system
} // namespace galaxias
