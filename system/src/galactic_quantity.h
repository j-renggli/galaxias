#pragma once

#include <math/quantity.h>

namespace galaxias
{
namespace system
{

template <class T>
class DerivedQuantity
{
public:
    DerivedQuantity(typename T::value_type x, double factor)
        : base_{x * factor}
        , factor_{factor}
    {
    }

    const T& base() const { return base_; }
    typename T::value_type value() const { return base_.value() / factor_; }

private:
    T base_;
    const double factor_;
};

class KiloLightYear : public DerivedQuantity<math::quantity::Metre>
{
public:
    static constexpr double factor{9.46073047258e+15};

    KiloLightYear(double x)
        : DerivedQuantity(x, factor)
    {
    }
};

} // namespace system
} // namespace galaxias
