/*
 * ContextMenu.h
 *
 *  Created on: Apr 15, 2014
 *      Author: paul
 */

#ifndef CONTEXTMENU_H_
#define CONTEXTMENU_H_

#include <ChibiEngine/UserInterface/ElementCollection.h>
#include <ChibiEngine/UserInterface/LookAndFeel.h>
#include <ChibiEngine/UserInterface/Button.h>

class SlotType;
class ContextMenuElemType;
namespace game {

	class ButtonContextMenu : public ElementCollection {
	public:
		ButtonContextMenu(ContextMenuElemType* iter, const std::string name);
		ButtonContextMenu(SlotType* iter);
		ButtonContextMenu();

		UIElement* add(const std::string& name);
		void remove(const std::string& name);
		inline float getAverageHeight() const;
		inline const std::string& getTypeName() override;

		void addSubMenu(const std::string& name, ButtonContextMenu* subMenu);
		void viewAt(const glm::vec2& pnt, float hw=-1);
		void hide();
		void hideAll(); // Скрывает не только себя, но и все родительские меню
		void hideOpenedFolder();
		void openSubMenu(ButtonContextMenu* menu);
		void executeLostFocus() override;
		void add(UIElement* elem) override;
		void setSize(const glm::vec2& _size) override;
		void draw() override;

	private:
		const static std::string TYPE_NAME;
		class DrawSimple;
		class DrawSprite;
		class ButtonWrap;
		class AbstractDraw{
		public:
			AbstractDraw(ButtonContextMenu& parent);
			virtual ~AbstractDraw() = default;

			virtual void setSize(const glm::vec2&){};
			virtual void draw() = 0;
			virtual void drawSelected(const glm::vec3& pos) = 0;

		protected:
			ButtonContextMenu& parent;
			MenuLAF laf;
		};

		AbstractDraw* getDrawer();
		ButtonContextMenu* createSubMenu(ContextMenuElemType* elem, ButtonContextMenu* parent);
		void setPosition2All(const glm::vec3& pnt, int direction, float hw=-1);

		const MenuLAF& laf;
		ButtonContextMenu* openedFolder;
		std::unique_ptr<AbstractDraw> drawer;
		float averageHeight;
		ButtonContextMenu* parent;
	};

	inline float ButtonContextMenu::getAverageHeight() const{
		return averageHeight;
	}

	inline const std::string& ButtonContextMenu::getTypeName(){
		return TYPE_NAME;
	}

}  // namespace game



#endif /* CONTEXTMENU_H_ */
