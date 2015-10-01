#ifndef __ROBOT_FX_OBJECT_H_
#define __ROBOT_FX_OBJECT_H_

#include <LevelObjects/LevelObject.h>
#include <ChibiEngine/Render/Particles/ParticleEffect.h>
#include <memory>
#include <ChibiEngine/Clock/Clock.h>

class FXType;
namespace game{

	class ParticleEffect;
	class FXObject : public LevelObject{
	public:
        FXObject(int uid, FXType* fx, Layer* layer);
		FXObject(int uid, Layer* layer, const EffectParams& parms,
				const std::string effectName, int zOrder, const std::string& name);
        void draw(CameraCHandle camera,const Color& ambientLight) override;
		void setColor(const Color& color) override;

        glm::vec3 getPosition() override;
	private:
        ParticleEffect effect;
		Clock::Time startTime;
	};

}

#endif
