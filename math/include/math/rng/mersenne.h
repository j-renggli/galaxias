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
    /// Construct an RNG with seed from system time
    Mersenne();
    /// Construct an RNG from a specific seed
    Mersenne(uint64_t seed);
    /// Construct an RNG from an existing RNG and a mask for the new seed (updates the reference)
    Mersenne(Mersenne& reference, uint64_t seedMask);
    virtual ~Mersenne() = default;

    /// Return a value in range [0, max)
    uint64_t uniform();
    /// Return a value within the given half-open range
    double uniform(const Range<double>& range);

private:
    std::mt19937_64 mt_;
};

} // namespace rng
} // namespace math
} // namespace galaxias
