#ifndef __RAIN_H_
#define __RAIN_H_

#include <ChibiEngine/Platform/Platform.h>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Render/Particles/ParticleEffect.h>
#include <ChibiEngine/Render/Particles/EffectTempl.h>
#include <ChibiEngine/Handles.h>
#include <glm/glm.hpp>

namespace game{

class RainTemplate : public EffectTemplate{
public:
    const static std::string NAME;

	RainTemplate();
	void draw(CameraCHandle camera, float dt_s, ParticleEffectCHandle effect) const override;
private:
    void generateBuffer() override;
    TextureCHandle texture;
};

}

#endif
