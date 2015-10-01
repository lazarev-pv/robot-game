#ifndef __EDITORINPUTLISTENER_H_
#define __EDITORINPUTLISTENER_H_

#include <glm/glm.hpp>
#include <string>
#include <ChibiEngine/Input/Input.h>
#include <ChibiEngine/Clock/EventSystem.h>

namespace game {

	class Event;
	class Logger;
	class EditorLevel;
	class Camera;
	class EditorInputListener : public RawEventListener {
	public:
		struct MouseRay{
			MouseRay(const glm::vec3& begin, const glm::vec3& end)
				:begin(begin),end(end){};
			glm::vec3 begin, end;

			glm::vec3 calcPos(float z) const;
			void draw(CameraCHandle camera) const;
		};

		EditorInputListener(EditorLevel* level);
		bool onClickDown(MouseButton code, const glm::vec2 &pnt)  override;
		bool onClickUp(MouseButton code, const glm::vec2 &pnt)  override;
		bool onMouseMove(const glm::vec2 &pnt) override;
		bool onKeyPressed(KeyCode keyCode, InputEvent::State state) override;
		bool onSpecialKeyPressed(KeyCode keyCode, InputEvent::State state) override;

		MouseRay getMouseRay() const;
		glm::vec2 getWorldCoordinatesZeroPlane() const;

		void draw(CameraCHandle camera);

	private:

		void scheduleSpecialRepeat(KeyCode keyCode);
		void deleteSpecialRepeatition();

		void hideMenus();
        void repeatKey();

		EditorLevel* level;
		MouseButton mouseCurrentButton;
        Logger logger;
        EventHandle keyRepeatEvent; // Указатель на эвент, чтобы при отпускании клавиши прекратить события удаления
        bool smoth;
        bool ctrlPressed;
        bool scalling;
        MouseRay mouseLastRay;

        glm::vec2 mouseCurrentPos;
        glm::vec2 mouseClickDownCoords;
		float nearZ, farZ;

        //
        KeyCode keyCode;
	};


}



#endif
