#pragma once

#include <deque>

namespace galaxias
{
namespace math
{
namespace rng
{

/// A class for known fixed probabilities, halving remainder forever if it's not complete
/// Usage: FixThenHalve({0.25, 0.75}) = 25% chance to fall in the 1st bin, 75% in the 2nd
template <class T>
struct FixThenHalve
{
    using value_type = T;

    FixThenHalve()
        : total_{0}
    {
    }
    FixThenHalve(std::initializer_list<T> init)
        : fix_{init}
        , total_{1}
    {
    }
    FixThenHalve(const FixThenHalve& rhs) = default;
    FixThenHalve(FixThenHalve&& rhs) noexcept = default;
    ~FixThenHalve() = default;

    FixThenHalve& operator=(const FixThenHalve& rhs) = default;
    FixThenHalve& operator=(FixThenHalve&& rhs) noexcept = default;

    bool operator!=(const FixThenHalve& rhs) const { return fix_ != rhs.fix_ || total_ != rhs.total_; }
    FixThenHalve& operator++()
    {
        if (fix_.empty())
        {
            total_ *= 0.5;
        }
        else
        {
            total_ -= fix_.front();
            fix_.pop_front();
        }
        return *this;
    }
    FixThenHalve operator++(int)
    {
        FixThenHalve copy{*this};
        ++(*this);
        return copy;
    }
    T operator*() { return fix_.empty() ? (total_ * 0.5) : fix_.front(); }

private:
    std::deque<T> fix_;
    T total_;
};

} // namespace rng
} // namespace math
} // namespace galaxias
