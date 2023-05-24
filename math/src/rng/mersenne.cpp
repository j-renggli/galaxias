#include <math/rng/mersenne.h>

namespace galaxias
{
namespace math
{
namespace rng
{

Mersenne::Mersenne()
    : mt_{std::random_device{}()}
{
}

Mersenne::Mersenne(uint64_t seed)
    : mt_{seed}
{
}

Mersenne::Mersenne(Mersenne& reference, uint64_t seedMask)
    : mt_{reference.uniform() ^ seedMask}
{
}

uint64_t Mersenne::uniform() { return mt_(); }

double Mersenne::uniform(const Range<double>& range)
{
    return std::uniform_real_distribution<>(range.low(), range.high())(mt_);
}

} // namespace rng
} // namespace math
} // namespace galaxias
