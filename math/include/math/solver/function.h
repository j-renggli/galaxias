#pragma once

namespace galaxias
{
namespace math
{
namespace solver
{

class IFunction
{
public:
    virtual ~IFunction() {}

    virtual double f(double x) const = 0;
    virtual double df(double x) const = 0;
};

} // namespace solver
} // namespace math
} // namespace galaxias
