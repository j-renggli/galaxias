#pragma once

#include <math/bounded_quantity.h>
#include <math/derived_quantity.h>

namespace galaxias
{
namespace system
{

namespace qty = math::quantity;
namespace ratio = math::unit::ratio;

class EarthRadius : public qty::DerivedQuantity<qty::BoundedMetre>
{
public:
    static constexpr double factor{6.3781e6};
    EarthRadius(double x)
        : DerivedQuantity(x, factor, math::Range<double>::positive())
    {
    }
};

class EarthMass : public qty::DerivedQuantity<qty::BoundedKilogram>
{
public:
    static constexpr double factor{5.9722e24};
    EarthMass(double x)
        : DerivedQuantity(x, factor, math::Range<double>::positive())
    {
    }
};

} // namespace system
} // namespace galaxias
