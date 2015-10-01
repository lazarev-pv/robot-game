/*
 * Button.h
 *
 *  Created on: Apr 15, 2014
 *      Author: paul
 */

#ifndef __SLIDER_H_
#define __SLIDER_H_

#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>

class SlotType;
class SliderLAF;
namespace game {

	class Slider : public UIElement {
	public:

        Slider(SlotType* iter);
        Slider(const glm::vec3& pos,
                float hw, float min, float max, float value,
                const std::string& name, int percision=-1);

        UIElement* containPoint(const glm::vec2& pnt) override;
        inline const std::string& getTypeName() override;
        void setSize(const glm::vec2& size) override;

        void setFloatValue(float val);
        void setPosition(const glm::vec3& pos) override;
        void processDraggingAction(const glm::vec2& delta) override;
        void draw() override;
        void setValue(const std::string& value) override;
        std::string getValue() const override;

	private:
        float getPositionByValue();
        float getValueByPosition();

        const static std::string TYPE_NAME;
        float min,max, value, markX;
        const SliderLAF& laf;
        int percision;
        SpriteText text;
	};

    inline const std::string& Slider::getTypeName(){
        return TYPE_NAME;
    }

}



#endif
