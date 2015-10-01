#ifndef __ROBOT_CHARACTER_TRANSFORM_LABEL_H_
#define __ROBOT_CHARACTER_TRANSFORM_LABEL_H_

#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/Render/Color.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include <ChedConstants.h>

namespace game{

    class TransformLine;
    class TexturedObject;
    class TransformLabel : public UIElement {
    public:
        TransformLabel(const std::string& label,
                const std::string& icon,
                Color bgColor,
                TransformId transformId,
                const glm::vec3& pos,
                const glm::vec2& size);

        void draw() override;
        virtual void executeAction() override;

        inline void setSelected(bool selected);

    private:
        TexturedObject* obj;
        SpriteText text;
        Color bgColor;
        TransformId transformId;
        bool selected = false;
    };

    inline void TransformLabel::setSelected(bool selected) {
        TransformLabel::selected = selected;
    }

}

#endif