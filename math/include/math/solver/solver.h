#pragma once

#include "../range.h"
#include "function.h"

namespace galaxias
{
namespace math
{
namespace solver
{

class ISolver
{
public:
    virtual ~ISolver() {}

    virtual double findRoot(const IFunction& fct, const Range<double>& range, const double tolerance = 1e-7) const = 0;
};

} // namespace solver
} // namespace math
} // namespace galaxias
