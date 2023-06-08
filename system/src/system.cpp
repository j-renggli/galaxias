#include <system/system.h>

#include "bodies/star.h"
#include "quantity/galactic.h"
#include <math/bounded_quantity.h>
#include <math/rng/fixed_proba.h>
#include <math/rng/prng.h>

#include <cassert>

namespace galaxias
{

namespace qty = math::quantity;

namespace system
{

namespace
{

// TODO: multiple masks?
constexpr uint64_t systemMask{0x2378A9CB3FEC95CULL};

using Rng = math::rng::Random;

std::string generateName(Rng&& dice)
{
    auto length = dice.uniform(math::Range<size_t>(4, 10));
    static constexpr auto vowels = "aeiouy";
    static constexpr auto consonants = "bcdfghjklmnpqrstvwxz";
    std::string name;
    name.resize(length);
    for (size_t i = 0; i < length; ++i)
    {
        const auto j = dice.uniform(math::Range<size_t>(0, i % 2 ? 6 : 20));
        const char* letters = i % 2 ? vowels : consonants;
        name[i] = letters[j];
    }
    name[0] += 'A' - 'a';
    return name;
}

} // namespace

class System : public ISystem
{
public:
    System(SystemIdentifier&& identifier);
    virtual ~System() {}

    void initialise();

    const orbit::coordinates::GalactoCentric& galacticCoords() const override { return identifier_.coordinates(); }

private:
    SystemIdentifier identifier_;
    const std::string name_;
};

System::System(SystemIdentifier&& identifier)
    : identifier_{std::move(identifier)}
    , name_{generateName(Rng{identifier_.dice(), systemMask})}
{
}

void System::initialise()
{
    // Determine number of stars
    Rng systemDice(identifier_.dice(), systemMask);
    const size_t starsCount =
        1 + systemDice.realisation(math::rng::FixThenHalve<float>({0.321, 0.479, 0.114, 0.044, 0.022, 0.010}),
                                   math::rng::FixThenHalve<float>());

    // Determine stars characteristics
    std::vector<std::shared_ptr<IBody>> bodies;
    for (size_t i = 0; i < starsCount; ++i)
    {
        bodies.emplace_back(std::make_shared<Star>(Rng{systemDice, systemMask}));
    }

    // Reorder stars (by mass?) randomly?

    // Add planets

    // Create hierarchy by grouping by weight of stars only (in-order) ??? Log distance ? Mind the SOI ?
}

std::unique_ptr<ISystem> ISystem::create(SystemIdentifier&& identifier)
{
    auto system = std::make_unique<System>(std::move(identifier));
    system->initialise();
    return system;
}

} // namespace system
} // namespace galaxias
