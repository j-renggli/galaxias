#pragma once

#include <math/derived_quantity.h>

namespace galaxias
{
namespace system
{

namespace qty = math::quantity;

class Parsec : public qty::DerivedQuantity<qty::Metre>
{
public:
    static constexpr double factor{3.0857e16};

    Parsec(double x)
        : DerivedQuantity{x, factor}
    {
    }
};

class LightYear : public qty::DerivedQuantity<qty::Metre>
{
public:
    static constexpr double factor{9.4607e15};

    LightYear(double x)
        : DerivedQuantity{x, factor}
    {
    }
};

class KiloLightYear : public qty::DerivedQuantity<qty::Metre>
{
public:
    static constexpr double factor{9.46073047258e18};

    KiloLightYear(double x)
        : DerivedQuantity{x, factor}
    {
    }
};

} // namespace system
} // namespace galaxias
