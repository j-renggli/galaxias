#pragma once

#include <stdexcept>

namespace galaxias
{
namespace math
{

template <class T>
class Range
{
public:
    using value_type = T;

    constexpr Range(const T& low, const T& high)
        : lo_{low}
        , hi_{high}
    {
        if (low >= high)
        {
            throw std::runtime_error("Range must have low < high; has " + std::to_string(low) +
                                     " >= " + std::to_string(high));
        }
    }

    const T& low() const noexcept { return lo_; }
    const T& high() const noexcept { return hi_; }
    T mid() const noexcept { return (lo_ + hi_) * 0.5; }

    bool includes(const T& value) const;

    void clamp(T& value) const;
    T clamp(const T& value) const;
    void modulo(T& value) const;
    T modulo(const T& value) const;

private:
    T lo_;
    T hi_;
};

} // namespace math
} // namespace galaxias

#include "range.inl"
