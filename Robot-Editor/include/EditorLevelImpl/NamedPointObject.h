#ifndef __NAMED_POINT_OBJECT_H_
#define __NAMED_POINT_OBJECT_H_

#include <EditorLevelImpl/PointObject.h>
#include <glm/glm.hpp>
#include "EditorConstants.h"

namespace game{

    class NamedPointObject : public PointObject {
    public:
        NamedPointObject(const glm::vec3& pos, const std::string& pointText,
                const std::string& className=EditorConstants::POINT);
        NamedPointObject(NamedPoint2D* pnt);
        virtual ~NamedPointObject();
        virtual void save2World(WorldType* world);
        virtual void draw(CameraCHandle camera);
        virtual OperableObject* clone();
        virtual void setSelected(bool isSelected);
        virtual void setName(const std::string& name);
        virtual OperableObject* containsPoint(const glm::vec2& pnt);

    protected:
        inline const std::string& getPointText() const;
        inline void setPointText(const std::string& name);
    private:
        SpriteText text;
        std::string className;
    };

    inline const std::string& NamedPointObject::getPointText() const{
        return text.getText();
    }

    inline void NamedPointObject::setPointText(const std::string& name){
        text.setText(name);
    }

}

#endif