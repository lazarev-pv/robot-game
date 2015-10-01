#ifndef __SPLASH_H_
#define __SPLASH_H_

#include <ChibiEngine/Platform/Platform.h>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Render/Particles/ParticleEffect.h>
#include <ChibiEngine/Render/Particles/EffectTempl.h>
#include <glm/glm.hpp>

namespace game{

class SplashTemplate : public EffectTemplate{
public:
    const static std::string NAME;

	SplashTemplate();
	void draw(CameraCHandle camera, float dt_s, ParticleEffectCHandle effect) const override;
private:
    void generateBuffer() override;
};

}



#endif
