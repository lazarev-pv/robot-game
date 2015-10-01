/*
 * Dialog.h
 *
 *  Created on: Apr 16, 2014
 *      Author: paul
 */

#ifndef UI_DIALOG_H_
#define UI_DIALOG_H_

#include <ChibiEngine/UserInterface/ElementCollection.h>
#include <ChibiEngine/UserInterface/LookAndFeel.h>
#include <ChibiEngine/UserInterface/Button.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>

class SlotType;
namespace game {

	class NineSliceSprite;
	class Dialog : public ElementCollection {
	public:
		Dialog(SlotType* iter);

		inline virtual const std::string& getTypeName() override;
		void draw() override;

	private:
		class Capture;

		const static std::string TYPE_NAME;

		const DialogLAF& laf;
		std::unique_ptr<NineSliceSprite> sprite;
	};

	inline const std::string& Dialog::getTypeName(){
		return TYPE_NAME;
	}


}  // namespace game



#endif /* DIALOG_H_ */
