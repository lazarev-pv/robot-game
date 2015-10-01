#ifndef __INPUT_SYSTEM_H
#define	__INPUT_SYSTEM_H

#include <vector>
#include <ChibiEngine/XMLParser/SettingsParser.h>
#include <map>
#include <glm/glm.hpp>
#include <ChibiEngine/Game.h>

namespace game{

	glm::vec3 convertToWorldCoordinates(glm::vec2 screenCoordinates);

    using KeyCode = int;
	enum class Cursor {
		USUAL = 0,
		HAND = 1,
		TEXT = 2
	};

	enum class MouseButton {
			NO_BUTTON =-1,
			LEFT=0,
			MIDDLE=1,
			RIGHT=2,
			SCROLL_UP=3,
			SCROLL_DOWN=4,
			SCROLL_LEFT=5,
			SCROLL_RIGHT=6};

	class InputEvent{
	public:
		using Handle = InputEvent*;

		enum State{PRESSED, RELEASED};
		virtual ~InputEvent(){};
		virtual void execute(const std::string& eventName, State state) = 0;
	};

	class RawEventListener{
	public:
		using Handle = RawEventListener*;

		static const KeyCode LEFT_ARROW = 100;
		static const KeyCode UP_ARROW = 101;
		static const KeyCode RIGHT_ARROW = 102;
		static const KeyCode DOWN_ARROW = 103;
		static const KeyCode LCTRL_KEY = 114;
		static const KeyCode RCTRL_KEY = 115;
		static const KeyCode LALT_KEY = 116;
		static const KeyCode RALT_KEY = 117;
		static const KeyCode LSHIFT = 112;
		static const KeyCode RSHIFT = 113;
		static const KeyCode DELETE_KEY = 127;

		static const KeyCode CTRL_A = 1;
		static const KeyCode CTRL_B = 2;
		static const KeyCode CTRL_C = 3;
		static const KeyCode CTRL_D = 4;
		static const KeyCode CTRL_E = 5;
		static const KeyCode CTRL_F = 6;
		static const KeyCode CTRL_G = 7;
		static const KeyCode CTRL_H = 8;
		static const KeyCode CTRL_I = 9;
		static const KeyCode CTRL_J = 10;
		static const KeyCode CTRL_K = 11;
		static const KeyCode CTRL_L = 12;
		static const KeyCode CTRL_M = 13;
		static const KeyCode CTRL_N = 14;
		static const KeyCode CTRL_O = 15;
		static const KeyCode CTRL_P = 16;
		static const KeyCode CTRL_Q = 17;
		static const KeyCode CTRL_R = 18;
		static const KeyCode CTRL_S = 19;
		static const KeyCode CTRL_T = 20;
		static const KeyCode CTRL_U = 21;
		static const KeyCode CTRL_V = 22;
		static const KeyCode CTRL_W = 23;
		static const KeyCode CTRL_X = 24;
		static const KeyCode CTRL_Y = 25;
		static const KeyCode CTRL_Z = 26;

		virtual ~RawEventListener() = default;
		/** if anyone returns true - processing stops */
		virtual bool onClickDown(MouseButton code, const glm::vec2 &pnt) = 0;
		virtual bool onClickUp(MouseButton code, const glm::vec2 &pnt) = 0;
		virtual bool onMouseMove(const glm::vec2 &pnt) = 0;
		virtual bool onKeyPressed(KeyCode keyCode, InputEvent::State state) = 0;
		virtual bool onSpecialKeyPressed(KeyCode keyCode, InputEvent::State state) = 0;
	};

	// todo может не стоит его привязать к Game
    class InputSystem : private UniqueClass {
    public:
		InputSystem(ProfileType* inputProfile);

		void mouseMoveEvent(const glm::vec2 &pnt);
        void clickDown(MouseButton code, const glm::vec2 &pnt);
        void clickUp(MouseButton code, const glm::vec2 &pnt);
        void keyPressed(KeyCode keyCode, InputEvent::State state);
        void keySpecialPressed(KeyCode keyCode, InputEvent::State state);

        void addRawListener(RawEventListener::Handle listener);
        void addEventListener(InputEvent::Handle event);

        void setMouseCursor(Cursor cursor);
		inline const glm::vec2& getCurrentMousePosition();
    private:
        static const KeyCode MAX_KEY_CODE = 255;
        bool isValidCode(KeyCode code);

		Logger logger;
        std::vector<RawEventListener::Handle> rawListeners;
        std::vector<InputEvent::Handle >inputEvents;
        std::map<KeyCode, std::string> keyMap;
		glm::vec2 currentMousePosition;
    };


	inline const glm::vec2& InputSystem::getCurrentMousePosition(){
		return currentMousePosition;
	}
}

#endif	
