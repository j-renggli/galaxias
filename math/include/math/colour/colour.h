#pragma once

#include "../quantity.h"

namespace galaxias
{
namespace math
{

class Colour
{
public:
    Colour(float r, float g, float b);
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

private:
    float r_;
    float g_;
    float b_;
};

Colour blackBodyColour(const quantity::Kelvin& temperature);

} // namespace math
} // namespace galaxias
