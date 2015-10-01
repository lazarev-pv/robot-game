#include <ChibiEngine/Render/Particles/EffectTempl.h>
#include <ChibiEngine/Render/Particles/ParticleEffect.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Common/ParserUtils.h>

using namespace game;
using namespace glm;
using namespace std;

EffectTemplate::EffectTemplate(const std::string& name)
    :name(name),inited(false){
}

ParticleEffect EffectTemplate::createEffect(const EffectParams& parms){
    if(!inited){
        generateBuffer();
        inited=true;
    }
    return ParticleEffect(this,parms);
}
