#include <math/colour/colour.h>

#include <stdexcept>

namespace galaxias
{
namespace math
{

namespace
{

constexpr float lim = 0.0031308f;
constexpr float g0 = 12.92f;
constexpr float g1 = 1.055f;
constexpr float g2 = 1.f / 2.4f;
constexpr float g3 = 0.055;
float gamma(float c) { return c <= lim ? (g0 * c) : (g1 * std::pow(c, g2) - g3); }

} // namespace

Colour::Colour(float r, float g, float b)
    : r_{r}
    , g_{g}
    , b_{b}
{
    if (r < 0. || r > 1. || g < 0. || g > 1. || b < 0. || b > 1.)
    {
        throw std::runtime_error("Colour must have values in range [0, 1]: " + std::to_string(r) + ", " +
                                 std::to_string(g) + ", " + std::to_string(b));
    }
}

Colour Colour::fromXYZ(float x, float y, float z)
{
    float r = gamma(3.2404542 * x - 1.5371385 * y - 0.4985314 * z);
    float g = gamma(-0.9692660 * x + 1.8760108 * y + 0.0415560 * z);
    float b = gamma(0.0556434 * x - 0.2040259 * y + 1.0572252 * z);

    // Enforce positivity
    const float w = -std::min(std::min(0.f, r), std::min(g, b));
    if (w > 0.f)
    {
        r += w;
        g += w;
        b += w;
    }

    // Normalise to the largest
    const float n = std::max(r, std::max(g, b));
    if (n > 0.f)
    {
        r /= n;
        g /= n;
        b /= n;
    }

    return Colour(r, g, b);
}

} // namespace math
} // namespace galaxias
