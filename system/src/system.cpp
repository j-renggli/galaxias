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
    //    std::shared_ptr<IBody> bodies_;
};

System::System(SystemIdentifier&& identifier)
    : identifier_{std::move(identifier)}
    , name_{generateName(Rng{identifier_.dice()})}
{
}

void System::initialise()
{
    // Determine number of stars
    Rng systemDice(identifier_.dice());
    const size_t starsCount =
        1 + systemDice.realisation(math::rng::FixThenHalve<float>({0.321, 0.479, 0.114, 0.044, 0.022, 0.010}),
                                   math::rng::FixThenHalve<float>());

    // Determine stars characteristics
    std::vector<std::shared_ptr<IBody>> bodies;
    for (size_t i = 0; i < starsCount; ++i)
    {
        bodies.emplace_back(std::make_shared<Star>(Rng{systemDice}));
    }

    // Reorder stars randomly, yielding a hierarchy for the system (A > B => B orbits A, A < B => B is new attractor)

    // Determine planets characteristics

    // Add planets

    // Create hierarchy by grouping by weight of stars only (in-order) ??? Log distance ? Mind the SOI ?
}

//// Prepare sky on separate thread
// if (level & PREPARE_SKY && ptrTransform != NULL)
//{
//  boost::shared_ptr<glscene::ITransform> ptrSkyTransform = ptrTransform->addChild();
//  ptrSkyTransform->setSelectable(false);
//  ptrSkyTransform->addShape(glscene::IShape::createSphere(2));
//  ptrSkyTransform->setShader(gShaderManager.getShader("sky"));
//  boost::shared_ptr<SkyMapper> ptrMapper(new SkyMapper(coords_.getPosition(), identifier, ptrSkyTransform));
//  common::thread::gThreadManager.sendEvent(ptrMapper);
////    ptrMapper->start();
//}

// ptrBarycenter_.reset(new Barycenter(boost::shared_ptr<orbitographie::Satellite>(), ptrTransform));

// if (level < PREPARE_STARS)
//  return core::rcSuccess;

//// 2. Now prepare the stars
// boost::shared_ptr<glscene::ITransform> ptrStarsTransform;
// boost::shared_ptr<glscene::ITransform> ptrPlanetsTransform;
// boost::shared_ptr<glscene::ITransform> ptrAsteroidsTransform;
// boost::shared_ptr<glscene::ITransform> ptrOrbitsTransform;
// boost::shared_ptr<glscene::IShape> ptrSphere;
// if (ptrTransform != NULL)
//{
//  ptrStarsTransform = ptrTransform->addChild(UNICODE_STRING_SIMPLE("stars"));
//  ptrStarsTransform->setShader(gShaderManager.getShader("star"));
//  ptrPlanetsTransform = ptrTransform->addChild();
//  ptrPlanetsTransform->setShader(gShaderManager.getShader("planet_water"));
//  ptrAsteroidsTransform = ptrTransform->addChild();
//  ptrAsteroidsTransform->setShader(gShaderManager.getShader("asteroid"));
//  ptrOrbitsTransform = ptrTransform->addChild(UNICODE_STRING_SIMPLE("orbits"));
//  ptrOrbitsTransform->setShader(gShaderManager.getShader("orbit"));

//  ptrSphere = glscene::IShape::createSphere(6);
//}

//// Create as many stars as necessary
// size_t stars = 0;
// double minMass = std::numeric_limits<double>::max();
// typedef std::vector<std::pair<boost::shared_ptr<Star>, boost::shared_ptr<glscene::ITransform> > > StarVector;
// StarVector vptrStars;
// core::LinearCongruential starDice(dice.getUniform());
// while (stars < 2)//6) // Hexary system is odd enough... !
//{
//  ++stars;

//  boost::shared_ptr<glscene::ITransform> ptrPosition;
//  boost::shared_ptr<glscene::ITransform> ptrOrientation;

//  if (ptrStarsTransform != NULL)
//  {
//    ptrPosition = glscene::ITransform::createTransform();//ptrStarsTransform->addChild();
//    ptrOrientation = ptrPosition->addChild();
//    ptrOrientation->addShape(ptrSphere);
//  }

//  boost::shared_ptr<Star> ptrStar;
//  rc = Star::createStar(ptrStar, *this, stars-1, ptrPosition, ptrOrientation, starDice.getUniform());//,
//  *ptrBarycenter_); if (rc.isFailure())
//    return rc;
////    ptrBarycenter_->addBody(ptrStar);
//  if (ptrOrientation != NULL)
//  {
//    const glscene::Colour col(ptrStar->getTemperature());
//    ptrOrientation->setParameter("colour", glscene::IShaderParameter::createColourParameter(col));
//    commonui::Image texture;
//    gTextureManager.generate("star", 512, 256, ptrStar->getParameter(), texture);
//    ptrOrientation->setParameter("texture", glscene::IShaderParameter::createTextureParameter(texture));
//    ptrOrientation->setParameter("radius",
//    glscene::IShaderParameter::createRealParameter(ptrStar->getApparentRadius(1.))); mT2B_[ptrOrientation.get()] =
//    ptrStar;
//  }

//  const double solarMass = ptrStar->getSolarMass();
//  StarVector::iterator it = vptrStars.begin();
//  for (; it != vptrStars.end(); ++it) {
//    if ((*it).first->getSolarMass() < solarMass) {
//      break;
//    }
//  }

//  vptrStars.insert(it, std::make_pair(ptrStar, ptrPosition));

//  if (level & PREPARE_ONLY_PRIMARY) {
//    ptrBarycenter_->addBody(vptrStars[0].first);
//    //ptrStarsTransform->addChild(vptrStars[0].second);
//    return core::rcSuccess;
//  }

//  if (ptrClock)
//    ptrClock->addMobile(ptrStar);

//  // 2.a. Chances of a multiple system: 0.12 * ln(M) + 0.33 where M is the smallest mass up to now
//  if (ptrStar->getSolarMass() < minMass) {
//    minMass = ptrStar->getSolarMass();
//  }
//  const double otherStar = starDice.getUniformDouble();
////    LOG_DEBUG(otherStar << " vs " << 0.12 * log(ptrStar->getSolarMass()) + 0.33);
////    if (otherStar > (0.12 * log(minMass) + 0.33))
////      break;
//}

// vptrStars_.clear();
// vptrStars_.reserve(vptrStars.size());
////  LOG_WARNING("Copying stars " << vptrStars.size());
// for (auto it = vptrStars.begin(); it != vptrStars.end(); ++it)
//{
//  vptrStars_.push_back(it->first);
//}

///// Let's randomise Dermott's law for object orbits (Sol: T0	34.542, C	2.28082 WITH CERES, with relative error
///~10%, 36% for Mars)
// double dermott_p1 = 0.;
// double dermott_p2 = 1.;
// dermott_p1 = dice.getGaussians(dermott_p2);
// double total_mu = 0.;
// for (size_t i = 0; i < vptrStars.size(); ++i)
//{
//  total_mu += vptrStars[i].first->getMass();
//}
// total_mu *= orbitographie::Constant::G;
// const double dermott_t0 = (1. + dermott_p1 / 6.) * 30. * sqrt(total_mu * 1e-20);
// const double dermott_c  = std::max(1.5, 3. + dermott_p2 * 0.5);
////  LOG_DEBUG("Dermott parameters for mu=" << total_mu << ": " << dermott_t0 << " & " << dermott_c);
// size_t dermott_index = 1;

//// Fix the center of masses
// if (ptrStarsTransform != NULL) {
////    if (vptrStars.size() == 1)
////    {
////      // Simplest case
////      ptrBarycenter_->addBody(vptrStars[0].first);
////      ptrStarsTransform->addChild(vptrStars[0].second);
////    } else {
////      if (vptrStars.size() % 2)
////      {
////        // Odd -> join middle star with next one
////        rc = prepareBinary(vptrStars[0].first, vptrStars[0].second, vptrStars[1].first, vptrStars[1].second,
////                           dermott_t0, dermott_c, dermott_index++, starDice, ptrBary, ptrTrf);
////      }
////    }
//  // TODO: Automatise task !!!
//  // 0 with size() / 2 or (size() / 2 and next one) if odd size
//  // Then it follows 1 with size() / 2 + 1, 2 with size() / 2 + 2, ... and build a tree such that balanced
//  //   0
//  // 0-1
//  //   0 - 1-2
//  // 0-2 - 1-3
//  //   0 - 2-3  -  1-4
//  // 0-3        -  1-4 - 2-5
//  //   0 - 3-4  -  1-5 - 2-6
//  // 0-4 - 3-7  -  1-5 - 2-6
//  // ...
//  switch (vptrStars.size()) {
//    case 1: {
//    ptrBarycenter_->addBody(vptrStars[0].first);
//    ptrStarsTransform->addChild(vptrStars[0].second);
//    break;
//  } case 2: {
//    boost::shared_ptr<Barycenter> ptrBary;
//    boost::shared_ptr<glscene::ITransform> ptrTrf;
//    rc = prepareBinary(vptrStars[0].first, vptrStars[0].second, vptrStars[1].first, vptrStars[1].second,
//                       dermott_t0, dermott_c, dermott_index++, starDice, ptrBary, ptrTrf);
//    if (rc.isFailure())
//      return rc;

//    ptrBarycenter_->addBody(ptrBary);
//    ptrStarsTransform->addChild(ptrTrf);
//    break;
//  } case 3: {
//    boost::shared_ptr<Barycenter> ptrBaryInternal;
//    boost::shared_ptr<glscene::ITransform> ptrTrfInternal;
//    rc = prepareBinary(vptrStars[2].first, vptrStars[2].second, vptrStars[1].first, vptrStars[1].second,
//                       dermott_t0, dermott_c, dermott_index++, starDice, ptrBaryInternal, ptrTrfInternal);
//    if (rc.isFailure())
//      return rc;

//    boost::shared_ptr<Barycenter> ptrBaryExternal;
//    boost::shared_ptr<glscene::ITransform> ptrTrfExternal;
//    rc = prepareBinary(vptrStars[0].first, vptrStars[0].second, ptrBaryInternal, ptrTrfInternal,
//                       dermott_t0, dermott_c, dermott_index++, starDice, ptrBaryExternal, ptrTrfExternal);
//    if (rc.isFailure())
//      return rc;

//    ptrBarycenter_->addBody(ptrBaryExternal);
//    ptrStarsTransform->addChild(ptrTrfExternal);
//    if (ptrClock)
//      ptrClock->addMobile(ptrBaryInternal);
//    break;
//  } case 4: {
//    boost::shared_ptr<Barycenter> ptrBaryLeft;
//    boost::shared_ptr<glscene::ITransform> ptrTrfLeft;
//    rc = prepareBinary(vptrStars[0].first, vptrStars[0].second, vptrStars[2].first, vptrStars[2].second,
//                       dermott_t0, dermott_c, dermott_index, starDice, ptrBaryLeft, ptrTrfLeft);
//    if (rc.isFailure())
//      return rc;

//    boost::shared_ptr<Barycenter> ptrBaryRight;
//    boost::shared_ptr<glscene::ITransform> ptrTrfRight;
//    rc = prepareBinary(vptrStars[1].first, vptrStars[1].second, vptrStars[3].first, vptrStars[3].second,
//                       dermott_t0, dermott_c, dermott_index++, starDice, ptrBaryRight, ptrTrfRight);
//    if (rc.isFailure())
//      return rc;

//    boost::shared_ptr<Barycenter> ptrBaryExternal;
//    boost::shared_ptr<glscene::ITransform> ptrTrfExternal;
//    rc = prepareBinary(ptrBaryLeft, ptrTrfLeft, ptrBaryRight, ptrTrfRight,
//                       dermott_t0, dermott_c, dermott_index++, starDice, ptrBaryExternal, ptrTrfExternal);
//    if (rc.isFailure())
//      return rc;

//    ptrBarycenter_->addBody(ptrBaryExternal);
//    ptrStarsTransform->addChild(ptrTrfExternal);

//    if (ptrClock)
//    {
//      ptrClock->addMobile(ptrBaryLeft);
//      ptrClock->addMobile(ptrBaryRight);
//    }
//    break;
//  } case 5: {
//    boost::shared_ptr<Barycenter> ptrBaryLeft;
//    boost::shared_ptr<glscene::ITransform> ptrTrfLeft;
//    rc = prepareBinary(vptrStars[2].first, vptrStars[2].second, vptrStars[3].first, vptrStars[3].second,
//                       dermott_t0, dermott_c, dermott_index, starDice, ptrBaryLeft, ptrTrfLeft);
//    if (rc.isFailure())
//      return rc;

//    boost::shared_ptr<Barycenter> ptrBaryRight;
//    boost::shared_ptr<glscene::ITransform> ptrTrfRight;
//    rc = prepareBinary(vptrStars[1].first, vptrStars[1].second, vptrStars[4].first, vptrStars[4].second,
//                       dermott_t0, dermott_c, dermott_index++, starDice, ptrBaryRight, ptrTrfRight);
//    if (rc.isFailure())
//      return rc;

//    boost::shared_ptr<Barycenter> ptrBaryMiddle;
//    boost::shared_ptr<glscene::ITransform> ptrTrfMiddle;
//    rc = prepareBinary(ptrBaryLeft, ptrTrfLeft, vptrStars[0].first, vptrStars[0].second,
//                       dermott_t0, dermott_c, dermott_index++, starDice, ptrBaryMiddle, ptrTrfMiddle);
//    if (rc.isFailure())
//      return rc;

//    boost::shared_ptr<Barycenter> ptrBaryExternal;
//    boost::shared_ptr<glscene::ITransform> ptrTrfExternal;
//    rc = prepareBinary(ptrBaryMiddle, ptrTrfMiddle, ptrBaryRight, ptrTrfRight,
//                       dermott_t0, dermott_c, dermott_index++, starDice, ptrBaryExternal, ptrTrfExternal);
//    if (rc.isFailure())
//      return rc;

//    ptrBarycenter_->addBody(ptrBaryExternal);
//    ptrStarsTransform->addChild(ptrTrfExternal);

//    if (ptrClock)
//    {
//      ptrClock->addMobile(ptrBaryLeft);
//      ptrClock->addMobile(ptrBaryMiddle);
//      ptrClock->addMobile(ptrBaryRight);
//    }
//    break;
//  } default: {
//    LOG_ERROR("Unhandled " << vptrStars.size());
//    return core::rcFailure;
//  }
//  }
//}

// if (level < PREPARE_PLANETS)
//  return core::rcSuccess;

//// 3. Now prepare the planets
// for (int i = 0; i < 5; ++i)
//{
//  boost::shared_ptr<glscene::ITransform> ptrPosition;
//  boost::shared_ptr<glscene::ITransform> ptrOrientation;
//  boost::shared_ptr<glscene::ITransform> ptrOrbit;

//  if (ptrPlanetsTransform != NULL)
//  {
//    ptrPosition = ptrPlanetsTransform->addChild();
//    ptrOrientation = ptrPosition->addChild();
//    ptrOrientation->addShape(ptrSphere);
//    ptrOrbit = ptrOrbitsTransform->addChild();
//  }

//  boost::shared_ptr<Planet> ptrPlanet;
//  rc = Planet::createPlanet(ptrPlanet, *this, i, ptrPosition, ptrOrientation, dice.getUniform(),
//                            dermott_t0, dermott_c, dermott_index++,
//                            //1.5e11 * (i+1), 1.5e11 * (i+2), //4.2e11,
//                            ptrBarycenter_->getGravitationalParam()); // Dists in m !!!
//  if (rc.isFailure())
//    return rc;

//  ptrPlanet->setOrbitTransform(ptrOrbit);
//  ptrBarycenter_->addBody(ptrPlanet);

//  if (ptrOrientation != NULL)
//  {
////      const glscene::Colour col(ptrStar->getTemperature());
////      ptrOrientation->setParameter("colour", glscene::IShaderParameter::createColourParameter(col));
//    commonui::Image texture;
//    gTextureManager.generate("planet_rock", 256, 128, ptrPlanet->getParameter(), texture);
//    ptrOrientation->setParameter("texture", glscene::IShaderParameter::createTextureParameter(texture));
//    ptrOrientation->setParameter("radius", glscene::IShaderParameter::createVectorParameter(maths::Vector<float>(2,
//    ptrPlanet->getApparentRadius(3.)))); mT2B_[ptrOrientation.get()] = ptrPlanet;

////          if (false)
////          {
////            boost::shared_ptr<glscene::ITransform> ptrTSky = transform.addChild();
////            ptrTSky->setShader(gShaderManager.getShader("weather"));
////            gTextureManager.generate("weather", 512, 256, ptrPlanet->getParameter(), texture);
////            ptrTSky->setParameter("texture", glscene::IShaderParameter::createTextureParameter(texture));
////            ptrTSky->addShape(ptrShape);
////          }
//    ASSERT(ptrClock != NULL);
//    ptrClock->addMobile(ptrPlanet);
//  }

std::unique_ptr<ISystem> ISystem::create(SystemIdentifier&& identifier)
{
    auto system = std::make_unique<System>(std::move(identifier));
    system->initialise();
    return system;
}

} // namespace system
} // namespace galaxias
