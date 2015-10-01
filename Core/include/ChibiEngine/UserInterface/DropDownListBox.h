/*
 * DropDownListBox.h
 *
 *  Created on: Apr 15, 2014
 *      Author: paul
 */

#ifndef __DROPDOWNLISTBOX_H_
#define __DROPDOWNLISTBOX_H_

#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/UserInterface/ContextMenu.h>

class SlotType;
namespace game {

	class ButtonContextMenu;
	class DropDownListBox : public Button{
	public:
		DropDownListBox(SlotType* iter);
		DropDownListBox(const std::string& text);

		UIElement* add(const std::string& name);
		std::string getValue() const override;
		inline const std::string& getTypeName() override;

		void remove(const std::string& name);
		void setElem(const std::string& name, bool fireEvent = false);
		void executeAction() override;
		void add2UI(UserInterface* ui) override;
		void executeLostFocus() override;
		void setVisible(bool flag) override;
        void setValue(const std::string& value) override;

	private:
		const static std::string TYPE_NAME;

		ButtonContextMenu* listPart;
	};


	inline const std::string& DropDownListBox::getTypeName(){
		return TYPE_NAME;
	}
}  // namespace game



#endif /* DROPDOWNLISTBOX_H_ */
