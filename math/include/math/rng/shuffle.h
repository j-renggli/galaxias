#pragma once

#include "../range.h"

namespace galaxias
{
namespace math
{
namespace rng
{

template <class It, class Rng>
void shuffle(const It& begin, const It& end, Rng& rng)
{
    auto it = begin;
    for (size_t size = std::distance(begin, end); size > 1; --size)
    {
        const size_t idx = rng.uniform(Range<size_t>{0, size - 1});
        if (idx != 0)
        {
            auto it2 = it + idx;
            std::swap(*it, *it2);
        }
        ++it;
    }
}

} // namespace rng
} // namespace math
} // namespace galaxias
