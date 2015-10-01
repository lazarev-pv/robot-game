#ifndef __FXOBJECT_H_
#define __FXOBJECT_H_

#include <ChibiEngine/Render/Particles/Rain.h>
#include <ChibiEngine/Render/Particles/Splash.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include <ChibiEngine/Render/Particles/ParticleEffect.h>
#include <EditorLevelImpl/BoxAreaObject.h>
#include <EditorLevelImpl/NamedPointObject.h>
#include <memory>

namespace game{

class FXObject : public BoxAreaObject {
public:
    FXObject(FXType* fxType,Layer* layer );
    FXObject(std::string effectName, const EffectParams& p,
            int zOrder=0,const std::string& name="", Layer* layer=nullptr);

    virtual void draw(CameraCHandle camera);
    virtual OperableObject* containsPoint(const glm::vec2& pnt);
    virtual void Move(const glm::vec3& delta);
    virtual void setSelected(bool isSelected);
    virtual void setPosition(const glm::vec3& position);
    virtual OperableObject* clone();
    virtual void save2World(WorldType* world){};

    void save2Layer(WorldLayers* layer) const;
    void setColor(const Color& color);
    const Color& getColor();


protected:
    NamedPointObject dirPnt;
	SpriteText text;
    ParticleEffect effect;
};

}



#endif
