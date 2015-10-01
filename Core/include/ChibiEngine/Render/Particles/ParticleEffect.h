#ifndef __PARTICLE_EFFECT_H_
#define __PARTICLE_EFFECT_H_

#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Render/Particles/EffectTempl.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Render/Particles/ParticleSystem.h>
#include <ChibiEngine/Common/ClassUtils.h>

namespace game{

    struct EffectParams;
    class ParticleEffect {
    public:
        inline ParticleEffect(EffectTemplateCHandle parent, const EffectParams& parms);
        ParticleEffect(const ParticleEffect& other) = default;
        ParticleEffect& operator=(const ParticleEffect& other) = default;

        inline void setSize(const glm::vec2& size);
        inline void move(const glm::vec3& delta);
        inline void setPosition(const glm::vec3& pos);
        inline void setDirection(const glm::vec2& pos);
        inline void setColor(const Color& color);
        inline void draw(CameraCHandle camera, float dt_s) const;

        inline const glm::vec3& getPosition() const;
        inline const glm::vec2& getDirection() const;
        inline const glm::vec2& getSize() const;
        inline const Color& getColor() const;
        inline EffectTemplateCHandle getTemplate() const;
    protected:
        EffectParams parms;
        EffectTemplateCHandle parent;
    };

    inline ParticleEffect::ParticleEffect(
            EffectTemplateCHandle parent,
            const EffectParams& parms)
        :parms(parms), parent(parent){
    }

    inline void ParticleEffect::setSize(const glm::vec2& size){
        parms.size.x=size.x;
        parms.size.y=size.y;
    }

    inline void ParticleEffect::setPosition(const glm::vec3& pos){
        parms.pos=pos;
    }

    inline void ParticleEffect::setDirection(const glm::vec2& dir){
        parms.dir=dir;
    }

    inline void ParticleEffect::move(const glm::vec3& delta){
        parms.pos+=delta;
    }

    inline void ParticleEffect::draw(CameraCHandle camera, float dt_s) const{
        parent->draw(camera, dt_s+2000, this);
    }

    inline EffectTemplateCHandle ParticleEffect::getTemplate() const{
        return parent;
    }

    inline const glm::vec3& ParticleEffect::getPosition() const{
        return parms.pos;
    }

    inline const glm::vec2& ParticleEffect::getDirection() const{
        return parms.dir;
    }

    inline const glm::vec2& ParticleEffect::getSize() const{
        return parms.size;
    }

    inline void ParticleEffect::setColor(const Color& color){
        parms.color = color;
    }

    inline const Color& ParticleEffect::getColor() const{
        return parms.color;
    }

}


#endif
