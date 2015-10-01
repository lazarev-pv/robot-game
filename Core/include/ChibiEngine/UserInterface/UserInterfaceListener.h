#ifndef __UI_LISTENER_H
#define	 __UI_LISTENER_H

#include <ChibiEngine/Input/Input.h>

//todo убрать из CoreAPI
namespace game{

	class UserInterface;
	class UserInterfaceListener : public RawEventListener {
	public:
		UserInterfaceListener(UserInterface* ui);
		bool onClickDown(MouseButton code, const glm::vec2 &pnt) override;
		bool onClickUp(MouseButton code, const glm::vec2 &pnt) override;
		bool onMouseMove(const glm::vec2 &pnt) override;
		bool onKeyPressed(KeyCode keyCode, InputEvent::State state) override;
		bool onSpecialKeyPressed(KeyCode keyCode, InputEvent::State state) override;
        inline const glm::vec2& getMousePos() const;
	private:
		UserInterface* ui;
        glm::vec2 mousePos;
	};

    inline const glm::vec2& UserInterfaceListener::getMousePos() const{
        return mousePos;
    }

}
#endif

