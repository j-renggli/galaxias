#pragma once

#include <math/derived_quantity.h>

namespace galaxias
{
namespace system
{

namespace qty = math::quantity;

class KiloLightYear : public qty::DerivedQuantity<qty::Metre>
{
public:
    static constexpr double factor{9.46073047258e+15};

    KiloLightYear(double x)
        : DerivedQuantity{x, factor}
    {
    }
};

} // namespace system
} // namespace galaxias
