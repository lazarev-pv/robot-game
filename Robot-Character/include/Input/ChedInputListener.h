#ifndef __ROBOT_CHARACTER_INPUT_CHEDINPUTLISTENER_H_
#define __ROBOT_CHARACTER_INPUT_CHEDINPUTLISTENER_H_

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <ChibiEngine/Input/Input.h>

namespace game{

	class Camera;
	class ChedLevel;
	class DropDownListBox;
	class ChedInputListener : public RawEventListener {
	public:
		class MouseStrategy;

		ChedInputListener(ChedLevel* level);
		virtual ~ChedInputListener();
		bool onClickDown(MouseButton code, const glm::vec2 &pnt) override;
		bool onClickUp(MouseButton code, const glm::vec2 &pnt) override;
		bool onMouseMove(const glm::vec2 &pnt) override;
		bool onKeyPressed(KeyCode keyCode, InputEvent::State state) override;
		bool onSpecialKeyPressed(KeyCode keyCode, InputEvent::State state) override;

		void draw(CameraCHandle camera);

		const glm::vec2& getCurrentPos();
	private:
        void onBoneContextMenu(const std::string& name);
		void onChedContextMenu(const std::string& name);


		ChedLevel* level;
		Logger logger;
		bool ctrlPressed;
		bool shiftPressed;
		MouseStrategy* nextStrat; // Для временных переключений режимов
		bool ignoreUp;// Игнорировать первый эвент кнопки вверх - полезно когда комнда приходит от
					  // контекстного меню или в середине предыдущей стратегии

		MouseStrategy *strat;
		std::map<std::string, MouseStrategy*> reference;
	};

}
#endif
