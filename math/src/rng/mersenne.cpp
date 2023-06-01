#include <math/rng/prng.h>

namespace galaxias
{
namespace math
{
namespace rng
{

template <>
Mersenne::PRNG(uint64_t seed)
    : generator_{seed}
{
}

} // namespace rng
} // namespace math
} // namespace galaxias
