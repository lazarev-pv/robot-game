#ifndef __EFFECT_TEMPLATE_H_
#define __EFFECT_TEMPLATE_H_

#include <ChibiEngine/Platform/Platform.h>
#include <ChibiEngine/Render/Camera.h>
#include <glm/glm.hpp>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Common/ClassUtils.h>
#include <memory>
#include <ChibiEngine/Handles.h>

class FXType;
namespace game{

    struct EffectParams;
    class EffectTemplate : private UniqueClass {
    public:
        EffectTemplate(const std::string& name);
        virtual ~EffectTemplate() = default;
        virtual void draw(CameraCHandle camera, float dt_s, ParticleEffectCHandle effect) const = 0;
        inline const std::string& getName() const;

        ParticleEffect createEffect(const EffectParams& parms);
    protected:
        mutable ShaderValues shaderValues;
    private:
        friend class ParticleSystem;
        virtual void generateBuffer()=0;

        std::string name;
        bool inited;

    };

    inline const std::string& EffectTemplate::getName() const{
        return name;
    }
}

#endif