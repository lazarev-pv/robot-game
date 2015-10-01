/*
 * TextLabel.h
 *
 *  Created on: Apr 15, 2014
 *      Author: paul
 */

#ifndef __TEXTLABEL_H_
#define __TEXTLABEL_H_


#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>

class SlotType;
namespace game {

	class TextLabel : public UIElement {
	public:
		TextLabel(const std::string& text);
		TextLabel(SlotType* slotType);

		void draw() override;
		void draw(const glm::vec3& position);
		void setValue(const std::string& text) override;

		std::string getValue() const override;
		inline const std::string& getTypeName() override;

	private:
		const static std::string TYPE_NAME;
		SpriteText spriteText;
		Color color;
	};

	inline const std::string& TextLabel::getTypeName(){
		return TYPE_NAME;
	}

}


#endif /* TEXTLABEL_H_ */
