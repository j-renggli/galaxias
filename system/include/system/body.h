#pragma once

#include <orbit/centerofmass.h>

#include <memory>

namespace galaxias
{

namespace qty = math::quantity;

namespace system
{

class IBody
{
public:
    virtual ~IBody() {}

    virtual qty::Kilogram mass() const = 0;

    virtual qty::Metre sphereOfInfluence() const = 0;

private:
    std::unique_ptr<orbit::CenterOfMass> com_;

    std::weak_ptr<IBody> parent_;
    std::vector<std::shared_ptr<IBody>> children_;
};

} // namespace system
} // namespace galaxias
