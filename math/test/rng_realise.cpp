#include <math/rng/prng.h>

#include <math/range.h>

#include <catch2/catch.hpp>

#include <deque>

using namespace galaxias;
using namespace math;
using namespace rng;

namespace
{

std::vector<float> quartiles{0.25, 0.25, 0.25, 0.25};

template <class T>
struct FixThenHalve
{
    using value_type = T;
    // difference_type, reference, pointer, and iterator_category

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

} // namespace

TEST_CASE("Realize function for quartiles")
{
    // Simple test with quartiles
    CHECK(detail::realise(quartiles.begin(), quartiles.end(), 0.) == 0);
    CHECK(detail::realise(quartiles.begin(), quartiles.end(), 0.1) == 0);
    CHECK(detail::realise(quartiles.begin(), quartiles.end(), 0.25) == 1);
    CHECK(detail::realise(quartiles.begin(), quartiles.end(), 0.4) == 1);
    CHECK(detail::realise(quartiles.begin(), quartiles.end(), 0.5) == 2);
    CHECK(detail::realise(quartiles.begin(), quartiles.end(), 0.6) == 2);
    CHECK(detail::realise(quartiles.begin(), quartiles.end(), 0.75) == 3);
    CHECK(detail::realise(quartiles.begin(), quartiles.end(), 0.9) == 3);
    CHECK_THROWS_AS(detail::realise(quartiles.begin(), quartiles.end(), 1.), std::out_of_range);
}

TEST_CASE("Realize function for fixed probabilities")
{
    FixThenHalve fth({0.2, 0.3, 0.4});
    CHECK(*fth == 0.2);
    ++fth;
    CHECK(*fth == 0.3);
    ++fth;
    CHECK(*fth == 0.4);
    ++fth;
    CHECK(*fth == Approx(0.05));
    ++fth;
    CHECK(*fth == Approx(0.025));
    ++fth;
    CHECK(*fth == Approx(0.0125));
    ++fth;

    CHECK(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.) == 0);
    CHECK(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.1) == 0);
    CHECK(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.2) == 1);
    CHECK(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.4) == 1);
    CHECK(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.5) == 2);
    CHECK(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.7) == 2);
    CHECK(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.9) == 3);
    CHECK(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.95) == 4);
    CHECK(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.975) == 5);
    CHECK(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.9875) == 6);
    CHECK(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.99999999) == 26);
    CHECK_THROWS_AS(detail::realise(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 1.), std::out_of_range);
}
