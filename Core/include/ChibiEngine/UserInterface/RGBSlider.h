/*
 * Button.h
 *
 *  Created on: Apr 15, 2014
 *      Author: paul
 */

#ifndef __RGB_SLIDER_H_
#define __RGB_SLIDER_H_

#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/UserInterface/ElementCollection.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>

class SlotType;
class SliderLAF;
namespace game {

	class Slider;
	class RGBSlider : public ElementCollection {
	public:
        RGBSlider(SlotType* iter);

        inline const std::string& getTypeName()override;
        std::string getValue() const override;

        void draw() override;
        void setSize(const glm::vec2& size) override;
        void setValue(const std::string& value)override;
        void setColor(const Color& val);
        void setPosition(const glm::vec3& pos)override;

	private:
        const static std::string TYPE_NAME;

        Color value;
        float colorHW;
        Slider *rSlider, *gSlider, *bSlider, *aSlider;
	};

    const std::string& RGBSlider::getTypeName(){
        return TYPE_NAME;
    }
}



#endif
