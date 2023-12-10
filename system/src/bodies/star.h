#pragma once

#include <math/colour/colour.h>
#include <math/rng/prng.h>
#include <system/body.h>

#include "../quantity/galactic.h"
#include "../quantity/solar.h"

namespace galaxias
{
namespace system
{

class Star : public IBody
{
public:
    enum class Classification
    {
        W,
        O,
        B,
        A,
        F,
        G,
        K,
        M,
        L,
        T,
        Y,
    };

    Star(math::rng::Random&& dice);
    ~Star() override {}

    qty::Kilogram mass() const override { return mass_.base(); }

    SolarRadius radius() const { return radius_; }

    Classification classification() const;

    qty::Kelvin temperature() const { return temperature_; }

    /// Star colour assumed from black body temperature
    math::Colour colour() const;

    SolarLuminosity luminosity() const { return luminosity_; }

    qty::Metre sphereOfInfluence() const override;

    BolometricMagnitude absoluteMagnitude() const;

    ApparentMagnitude apparentMagnitude(const Parsec& distance) const;

private:
    /// Mass (prime value. All other derive from this one +- some noise)
    const SolarMass mass_;

    /// Radius of the star (useful for display)
    const SolarRadius radius_;

    /// Effective temperature (black body)
    const qty::BoundedKelvin temperature_;

    /// Luminosity (for absolute -> apparent magnitude)
    const SolarLuminosity luminosity_;

    //    /// A parameter, specific for this star only
    //    double parameter_;

    //    /// Bodies in the sphere of influence
    //    std::vector<boost::shared_ptr<IAttractor> > vptrSatellites_;

    //    /// Position w.r.t. other stars
    //    size_t position_;

    //    /// The text to display when hovering over this star
    //    UnicodeString strName_;
    //    UnicodeString strDescription_;
};

} // namespace system
} // namespace galaxias
