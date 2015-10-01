
#include <LevelObjects/FXObject.h>
#include <ChibiEngine/Render/Particles/ParticleSystem.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Clock/Clock.h>
#include <ChibiEngine/Render/Particles/ParticleEffect.h>

using namespace game;
using namespace glm;
using namespace std;

FXObject::FXObject(int uid, FXType* fx, Layer* layer)
        :FXObject(uid, layer, EffectParams(fx), fx->effect, fx->zOrder, fx->name){
}

FXObject::FXObject(int uid, Layer* layer, const EffectParams& parms,
        const std::string effectName, int zOrder, const std::string& name)
    :LevelObject(uid, layer, zOrder, name),
     effect(Game::getParticleSystem()->createEffect(effectName,parms)),
     startTime(Game::getLevelClock()->getTime()){
}

void FXObject::draw(CameraCHandle camera, const Color& ambientLight){
    if(isEnabled()){
        float dt_s = (Game::getLevelClock()->getTime()-startTime)*0.001f;
        //Game::getLogger().error(" ut %f, st %f",dt_s, fmod(dt_s,0.75f));
        effect.draw(camera,dt_s);
    }
}

glm::vec3 FXObject::getPosition(){
    return effect.getPosition();
}

void FXObject::setColor(const Color& color){
    effect.setColor(color);
}