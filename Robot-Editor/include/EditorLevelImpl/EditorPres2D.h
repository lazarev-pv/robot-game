#ifndef __EDITOR_LEVEL_OBJECT_PRESENTATION_H_
#define __EDITOR_LEVEL_OBJECT_PRESENTATION_H_

#include <EditorLevelImpl/BoxAreaObject.h>
#include <EditorLevel.h>
#include <string>
#include "EditorGeometryObject.h"
#include <memory>


class ArtType;
class WorldLayers;
namespace game{

class EditorFixtureObject;
class TexturedObject;
class Layer;
class PatternSprite;
class EditorPres2D : public BoxAreaObject{
public:

	EditorPres2D(
            const glm::vec3& offset,
            const std::string& spriteName,
            const glm::vec2& scale,
            EditorFixtureObject * levelObject,
            Layer* layer);
	EditorPres2D(ArtType* art, EditorFixtureObject * obj, Layer* layer);
	virtual ~EditorPres2D();

    void setScale(const glm::vec2& scale);
    glm::vec2 getScale() const;
	void spreadTexture(const glm::vec2& sizeDelta);
    inline bool isSpreadMode() const;
    inline void setSpreadMode(bool isOn);
    ArtType* save2ArtType() const;
    const std::string& getSpriteName() const;

    EditorFixtureObject * getParent();
    virtual void save2Layer(WorldLayers* layer) const;
	virtual void draw(CameraCHandle camera);
	virtual OperableObject* clone();
	virtual void setSelected(bool isSelected);
    virtual void Move(const glm::vec3& delta);


    inline void setColor(const Color& color);
    inline const Color& getColor() const;
private:
    EditorFixtureObject * levelObject;
    std::unique_ptr<PatternSprite> sprite;
    bool spread;
    glm::vec2 scale;
    Color color=Color::White;
};

    inline bool EditorPres2D::isSpreadMode() const{
        return EditorPres2D::spread;
    }

    inline void EditorPres2D::setSpreadMode(bool isOn){
        EditorPres2D::spread=isOn;
    }

    inline void EditorPres2D::setColor(const Color& color){
        EditorPres2D::color = color;
    }

    inline const Color& EditorPres2D::getColor() const{
        return EditorPres2D::color;
    }

}



#endif
