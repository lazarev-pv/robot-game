#include <ChibiEngine/UserInterface/UserInterfaceListener.h>
#include <ChibiEngine/UserInterface/UserInterface.h>

using namespace game;
using namespace glm;
using namespace std;

//todo перенести сюда логику отслеживания нажатий и движения
UserInterfaceListener::UserInterfaceListener(UserInterface* ui):
		ui(ui){}

bool UserInterfaceListener::onClickDown(MouseButton code, const glm::vec2 &pnt){
	if(code == MouseButton::LEFT) {
        return ui->click(pnt, UserInterface::CLICK_DOWN);
    } else if(code == MouseButton::SCROLL_UP){
        return ui->scroll(true);
    } else if(code == MouseButton::SCROLL_DOWN){
        return ui->scroll(false);
    } else {
        return false;
    }
}

bool UserInterfaceListener::onClickUp(MouseButton code, const glm::vec2 &pnt){
	return code == MouseButton::LEFT? ui->click(pnt, UserInterface::CLICK_UP):false;
}

bool UserInterfaceListener::onMouseMove(const glm::vec2 &pnt){
    mousePos = pnt;
	return ui->moveMouseEvent(pnt);
}

bool UserInterfaceListener::onKeyPressed(KeyCode keyCode, InputEvent::State state){
	return ui->onKeyPressed(keyCode, state, false);
}

bool UserInterfaceListener::onSpecialKeyPressed(KeyCode keyCode, InputEvent::State state){
	return ui->onKeyPressed(keyCode, state, true);
}
