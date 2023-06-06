#pragma once

#include "quantity.h"

namespace galaxias
{
namespace math
{
namespace quantity
{

template <class T>
class DerivedQuantity
{
public:
    using Base = T;

    template <class... P>
    DerivedQuantity(typename T::value_type x, double factor, const P&... params)
        : base_{x * factor, params...}
        , factor_{factor}
    {
    }

    const Base& base() const { return base_; }
    typename Base::value_type value() const { return base_.value() / factor_; }

private:
    Base base_;
    const double factor_;
};

} // namespace quantity
} // namespace math
} // namespace galaxias
