#include <iostream>

#include <orbit/centerofmass.h>
#include <system/system_identifier.h>
#include <ui/ui.h>

using namespace galaxias;
namespace qty = math::quantity;

int main(int argc, char* argv[])
{
    //    const qty::Second time0{0.};
    //    const orbit::coordinates::Cartesian coord0{{{1e9, 0., 0.}}, {{0., 1e3, 0.}}};
    //    const orbit::GravitationalParam muCenter{1e15};
    //    auto root = std::make_shared<orbit::CenterOfMass>(muCenter);
    //    auto a = std::make_shared<orbit::CenterOfMass>(muCenter, time0, coord0, root);
    //    //    std::cout << a->orbitalPeriod().high() << std::endl;
    //    const double step = a->orbitalPeriod().high() / 10.;
    //    for (double t = step; t < a->orbitalPeriod().high() * 1.05; t += step)
    //    {
    //        auto c1 = root->coordinatesAt(qty::Second(t));
    //        auto c2 = a->coordinatesAt(qty::Second(t));
    //        std::cout << c1 << std::endl;
    //        std::cout << c2 << std::endl;
    //  }

    //    auto s0 = galaxias::system::SystemIdentifier::fromValue(0);
    //    auto s1 = galaxias::system::SystemIdentifier::fromValue(1);
    //    auto s2 = galaxias::system::SystemIdentifier::fromValue(2);

    auto ui = IUI::create(argc, argv);
    return ui->run();

    return 0;
}
