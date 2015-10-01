#ifndef __UI_INPUT_FIELD_H_
#define __UI_INPUT_FIELD_H_

#include <ChibiEngine/UserInterface/UIElement.h>
#include <string>
#include <ChibiEngine/Render/Primitives/SpriteText.h>


class SlotType;
namespace game {

	class InputField : public UIElement{
	public:

		InputField(SlotType* iter);
		InputField(const std::string& value="");

		void onKeyPressed(KeyCode keyCode, bool special) override;
		void executeViewCursor(bool visible) override;
		void draw() override;
		void executeLostFocus() override;
		void executeFocused() override;
		void executeHoverOnAnimation() override;
		void executeHoverOutAnimation() override;
		void setValue(const std::string& value) override;

		inline const std::string& getTypeName();
		inline std::string getValue() const override;
	private:
		const static std::string TYPE_NAME;

		class AbstractDrawer {
		public:
			AbstractDrawer(InputField* parent);
			virtual ~AbstractDrawer() = default;
			virtual void draw() = 0;
			glm::vec3 getTextPos();
		protected:
			InputField* parent;
			const InputFieldLAF& laf;
		};

		class SimpleDrawer : public InputField::AbstractDrawer {
		public:
			SimpleDrawer(InputField* parent);
			virtual void draw();
		};

		void deleteLast();
		void moveCursor(int delta);
		void acceptCode(unsigned char code);

		std::function<bool(char, const std::string& )> filter;
		bool viewCursor = false;
		bool selection = false;
		bool focused = false;
		size_t maxSize;
		std::string value;
		SpriteText textPart;
		int cursorPosition = 0;
		SimpleDrawer drawer;
	};

    const std::string& InputField::getTypeName(){
        return TYPE_NAME;
    }

    std::string InputField::getValue() const{
        return value;
    }
}  // namespace game

#endif
