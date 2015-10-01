#ifndef __LIGHT_SOURCE_H_
#define __LIGHT_SOURCE_H_

#include <glm/glm.hpp>
#include <ChibiEngine/Render/Sprites/ArbitrarySprite.h>
#include <LevelObjects/LevelObject.h>

class LightType;
namespace game{

    class Camera;
    class LightSource : public LevelObject {
    public:
        static LightSource* create(int uid, const LightType* l, Layer* layer);

        inline virtual void draw(CameraCHandle camera, const Color &ambientLight);
        virtual void draw(CameraCHandle camera)= 0;

        inline void setColor(const Color& color) override;

    protected:
        LightSource(int uid, Layer* layer, int zOrder, const std::string& name);
        inline virtual ~LightSource();
        Color color;
    };

    class TexturedObject;
    class SpotLightSource : public LightSource {
    public:
        SpotLightSource(int uid, const LightType* l,Layer* layer);
        virtual void draw(CameraCHandle camera);

        inline virtual glm::vec3 getPosition();
    protected:
        inline virtual ~SpotLightSource();

    private:
        TexturedObject* pres;
        float intensity;
        glm::vec3 pos;
        glm::vec2 size;
    };

    class ArbitrarySprite;
    class DirLightSource : public LightSource {
    public:
        DirLightSource(int uid, const LightType* l,Layer* layer);
        virtual void draw(CameraCHandle camera);

        virtual glm::vec3 getPosition();
    protected:
        virtual ~DirLightSource();

    private:
        ArbitrarySprite* pres;
    };

    inline void LightSource::setColor(const Color& color){
        LightSource::color = color;
    }

    inline LightSource::~LightSource(){}

    inline SpotLightSource::~SpotLightSource(){}

    inline void LightSource::draw(CameraCHandle camera, const Color &ambientLight){}

    inline glm::vec3 SpotLightSource::getPosition(){
        return pos;
    }
}

#endif