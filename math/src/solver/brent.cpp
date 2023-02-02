#include <math/solver/brent.h>

namespace galaxias
{
namespace math
{
namespace solver
{

double Brent::findRoot(const IFunction& fct,
                       const Range<double>& range,
                       const double tolerance) const
{
    double x0 = range.low();
    double y0 = fct.f(x0);
    double x1 = range.high();
    double y1 = fct.f(x1);
    const double tol2 = tolerance * tolerance;

    if (y0 * y0 <= tol2)
    {
        return x0;
    }
    if (y1 * y1 <= tol2)
    {
        return x1;
    }

    if (y0 * y1 > 0.)
    {
        throw std::runtime_error("Both limits evaluate to positive or "
                                 "negative, won't search for a root here");
    }

    if (y0 * y0 < y1 * y1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    double x2 = x0;
    bool bFlag = true;
    double x = x1;
    double ys = y1;
    double y2 = y1;
    double d = 0;

    while (y1 != 0)
    {
        if (std::abs(x0 - x1) < tolerance)
            break;

        if (y2 != y0 && y2 != y1)
            x = (x0 * y1 * y2) / ((y0 - y1) * (y0 - y2)) +
                (x1 * y0 * y2) / ((y1 - y0) * (y1 - y2)) +
                (x2 * y0 * y1) / ((y2 - y0) * (y2 - y1));
        else
            x = x1 - y1 * (x1 - x0) / (y1 - y0);

        const double ab34 = (3. * x0 + x1) * 0.25;
        bool bClear = true;
        if ((x < ab34 && x < x1) || (x > ab34 && x > x1))
        {
            bClear = false;
        }
        else if (bFlag)
        {
            const double bmc = y1 - y2;
            const double smb = x - y1;
            if (bmc * bmc < tol2 || smb * smb >= bmc * bmc * 0.25)
                bClear = false;
        }
        else
        {
            const double cmd = y2 - d;
            const double smb = x - y1;
            if (cmd * cmd < tol2 || smb * smb >= cmd * cmd * 0.25)
                bClear = false;
        }
        if (bClear)
            bFlag = false;
        else
        {
            x = (x0 + x1) * 0.5;
            bFlag = true;
        }

        ys = fct.f(x);
        ;
        d = x2;
        x2 = x1;
        y2 = y1;
        if (y0 * ys < 0)
        {
            x1 = x;
            y1 = ys;
        }
        else
        {
            x0 = x;
            y0 = ys;
        }

        if (y0 * y0 < y1 * y1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
    }

    return x1;
}

} // namespace solver
} // namespace math
} // namespace galaxias
