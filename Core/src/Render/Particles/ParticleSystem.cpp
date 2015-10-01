#include <ChibiEngine/Render/Particles/ParticleSystem.h>
#include <ChibiEngine/Render/Particles/Rain.h>
#include <ChibiEngine/Render/Particles/Splash.h>
#include <ChibiEngine/Render/Particles/Fire.h>
#include <ChibiEngine/Render/Particles/Lightning.h>
#include <ChibiEngine/Render/Particles/Explosion.h>
#include <ChibiEngine/Render/Particles/Steam.h>
#include <ChibiEngine/Common/RAIIUtils.h>
#include <ChibiEngine/XMLParser/WorldParser.h>

using namespace glm;
using namespace game;
using namespace std;

ParticleSystem::ParticleSystem(){
    effects[RainTemplate::NAME]=make_unique<RainTemplate>();
    effects[SplashTemplate::NAME]=make_unique<SplashTemplate>();
    effects[FireTemplate::NAME]=make_unique<FireTemplate>();
    effects[LightningTemplate::NAME]=make_unique<LightningTemplate>();
    effects[SteamTemplate::NAME]=make_unique<SteamTemplate>();
    effects[ExplosionTemplate::NAME]=make_unique<ExplosionTemplate>();
}

ParticleEffect ParticleSystem::createEffect(const std::string& name, const EffectParams& parms) {
    return effects[name]->createEffect(parms);
}

