#ifndef __PARTICLESYSTEM_H_
#define __PARTICLESYSTEM_H_

#include <glm/glm.hpp>
#include <map>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/Common/ClassUtils.h>
#include <ChibiEngine/Handles.h>

namespace game{

    struct EffectParams{
        EffectParams(const glm::vec3& pos,
                const glm::vec2& size,
                const glm::vec2& dir,
                const Color& color):
                pos(pos),size(size),dir(dir),color(color){}

        EffectParams(FXType* fx):
                pos(fx->x,fx->y,0),
                size(fx->hw,fx->hh),
                dir(parse(fx->dir)),
                color(parse(fx->color)){}

        glm::vec3 pos;
        glm::vec2 size;
        glm::vec2 dir;
        Color color;
    };

    class ParticleSystem : private UniqueClass {
    public:
        ParticleSystem();

        ParticleEffect createEffect(const std::string& name, const EffectParams& parms);
    private:
        std::map<std::string, std::unique_ptr<EffectTemplate>> effects;
    };

}



#endif
