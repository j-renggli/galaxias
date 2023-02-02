#pragma once

#include "../range.h"

#include <random>

namespace galaxias
{
namespace math
{
namespace rng
{

class Mersenne
{
public:
    Mersenne();
    Mersenne(uint64_t seed);
    virtual ~Mersenne() = default;

    double uniform(const Range<double>& range);

private:
    std::mt19937_64 mt_;
};

} // namespace rng
} // namespace math
} // namespace galaxias
