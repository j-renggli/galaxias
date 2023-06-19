#pragma once

#include "../quantity.h"

#include <string>

namespace galaxias
{
namespace math
{

class Colour
{
public:
    Colour(float r, float g, float b, float a = 1.f);
    Colour() = delete;
    Colour(const Colour&) = default;
    Colour(Colour&&) noexcept = default;
    ~Colour() = default;

    Colour& operator=(const Colour&) = default;
    Colour& operator=(Colour&&) noexcept = default;

    static Colour fromXYZ(float x, float y, float z);

    float r() const { return r_; }
    float g() const { return g_; }
    float b() const { return b_; }
    float a() const { return a_; }

    std::string hex() const;

private:
    float r_;
    float g_;
    float b_;
    float a_;
};

Colour blackBodyColour(const quantity::Kelvin& temperature);

} // namespace math
} // namespace galaxias
