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

double Mersenne::uniform(const Range<double>& range)
{
    return std::uniform_real_distribution<>(range.low(), range.high())(mt_);
}

} // namespace rng
} // namespace math
} // namespace galaxias
