#include <math/rng/prng.h>

namespace galaxias
{
namespace math
{
namespace rng
{

namespace
{

static inline uint64_t rotl(const uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }

uint64_t splitmix64(uint64_t& x)
{
    // See https://prng.di.unimi.it/splitmix64.c
    uint64_t z = (x += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}

} // namespace

namespace detail
{

Xoshiro256::Xoshiro256(uint64_t seed)
{
    // Use splitmix64 to initialise the array from this seed
    s_[0] = seed;
    s_[1] = splitmix64(seed);
    s_[2] = splitmix64(seed);
    s_[3] = splitmix64(seed);
}

uint64_t Xoshiro256::operator()()
{
    // See https://prng.di.unimi.it/xoshiro256starstar.c
    const uint64_t result = rotl(s_[1] * 5, 7) * 9;
    const uint64_t t = s_[1] << 17;

    s_[2] ^= s_[0];
    s_[3] ^= s_[1];
    s_[1] ^= s_[2];
    s_[0] ^= s_[3];
    s_[2] ^= t;
    s_[3] = rotl(s_[3], 45);

    return result;
}

} // namespace detail

template <>
Random::PRNG(uint64_t seed)
    : generator_{seed}
{
}

} // namespace rng
} // namespace math
} // namespace galaxias
