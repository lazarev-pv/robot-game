#ifndef __CHIBIENGINE_PARTICLES_PARTICLE_HANDLES_
#define __CHIBIENGINE_PARTICLES_PARTICLE_HANDLES_

namespace game{

    class ParticleEffect;
    using ParticleEffectHandle = ParticleEffect*;
    using ParticleEffectCHandle = const ParticleEffect*;

    class EffectTemplate;
    using EffectTemplateHandle = EffectTemplate*;
    using EffectTemplateCHandle = const EffectTemplate*;

    class TexturedObject;
    using TexturedObjectHandle = TexturedObject*;
    using TexturedObjectCHandle = const TexturedObject*;

    class AnimatedSprite;
    using AnimatedSpriteHandle = AnimatedSprite*;

    class Camera;
    using CameraHandle = Camera*;
    using CameraCHandle = const Camera*;

    using ScreenFrameId = int;
}

#endif