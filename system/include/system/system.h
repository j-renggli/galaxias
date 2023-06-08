#pragma once

#include "body.h"
#include "system_identifier.h"

#include <memory>

namespace galaxias
{
namespace system
{

class ISystem
{
public:
    virtual ~ISystem() {}

    static std::unique_ptr<ISystem> create(SystemIdentifier&& identifier);

    virtual const orbit::coordinates::GalactoCentric& galacticCoords() const = 0;
};

} // namespace system
} // namespace galaxias
