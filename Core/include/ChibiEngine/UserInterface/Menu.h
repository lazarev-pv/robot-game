#ifndef __UI_MENU_H_
#define __UI_MENU_H_

#include <ChibiEngine/UserInterface/ElementCollection.h>
#include <vector>
#include <string>

class MenuType;
class ContextMenuElemType;
namespace game{

	class ButtonContextMenu;
	class TextLabel;
	class NineSliceSprite;
	class Menu : public ElementCollection{
	public:
		Menu(MenuType* iter);

		inline const std::string& getTypeName() override;
		void draw() override;

	private:
		const static std::string TYPE_NAME;
		class Button;

		void openAt(int i);
		void closeAll();

		struct Element{
			Element(const std::string& name, const std::string& parentName, ContextMenuElemType* iter, int num, Menu* parent);
			Button* label;
			ButtonContextMenu* subMenu;
		};

		std::vector<Element> subMenues;
		std::unique_ptr<NineSliceSprite> back;
		int openedNum;
	};

	inline const std::string& Menu::getTypeName(){
		return TYPE_NAME;
	}

}



#endif
