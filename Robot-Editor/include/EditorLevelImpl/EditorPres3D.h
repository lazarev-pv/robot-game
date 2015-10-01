#ifndef __BOX_3D_PRES_
#define __BOX_3D_PRES_

#include <EditorLevel.h>
#include <string>
#include "BoxAreaObject.h"

class Box3DType;
class ArtType;
class WorldLayers;
namespace game{

    class Block;
    class Layer;
    class EditorGeometryObject;
    class Texture;
    class EditorPres3D : public BoxAreaObject{
    public:

        EditorPres3D(Box3DType* iter, Layer* layer);
        EditorPres3D(const glm::vec3& pos,
                const Texture* tex,
                Layer* layer,
                const glm::vec3& size=glm::vec3(1,0.5,0.5),
                const glm::vec2& texes=glm::vec2(1,1),
                const std::string& name="",
                const Color& color=Color::White);
        virtual ~EditorPres3D();

        void setTexels(const glm::vec2& tex);
        void save2Layer(WorldLayers* layer) const;
        const glm::vec2& getTexels() const;
        inline bool isSpreadMode() const;
        inline void setSpreadMode(bool isOn);

        virtual void draw(CameraCHandle camera);
        virtual OperableObject* clone();
        virtual void setSelected(bool isSelected);
        EditorGeometryObject * getParent(){
            return nullptr;
        }

        inline void setColor(const Color& color);
        inline const Color& getColor() const;
    private:

        Block* sprite;
        const Texture* tex;
        float hd;
        bool spread;
        glm::vec2 scale;
        Color color;
    };


    inline bool EditorPres3D::isSpreadMode() const{
        return EditorPres3D::spread;
    }

    inline void EditorPres3D::setSpreadMode(bool isOn){
        EditorPres3D::spread=isOn;
    }

    inline void EditorPres3D::setColor(const Color& color){
        EditorPres3D::color = color;
    }

    inline const Color& EditorPres3D::getColor() const{
        return EditorPres3D::color;
    }
}



#endif
