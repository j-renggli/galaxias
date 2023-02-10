#pragma once

#include "orbital_elements.h"
#include "position.h"
#include <math/bounded_quantity.h>

#include <memory>

namespace galaxias
{
namespace orbit
{

namespace qty = math::quantity;

using GravitationalParam =
    qty::BoundedQuantity<double, math::unit::Unit<math::unit::ratio::NegTwo, math::unit::ratio::Three>>;

class CenterOfMass
{
public:
    CenterOfMass(const GravitationalParam& mu);
    CenterOfMass(const GravitationalParam& mu,
                 const qty::Second& time0,
                 const coordinates::Cartesian& coord0,
                 const std::shared_ptr<CenterOfMass>& parent);

    const OrbitalElements& orbitalElements() const { return oe_; }

private:
    GravitationalParam mu_;
    qty::Second t0_;
    coordinates::Cartesian coord0_;
    OrbitalElements oe_;
    std::shared_ptr<CenterOfMass> parent_;
};

} // namespace orbit
} // namespace galaxias
