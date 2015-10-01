#ifndef __GAMESTATE_H
#define	 __GAMESTATE_H

#include <ChibiEngine/Physics/Physics.h>
#include <ChibiEngine/Common/EventSupportable.h>
#include <ChibiEngine/Log/Logger.h>
#include <ChibiEngine/Common/EventSupportable.h>
#include "FpsCounter.hpp"

namespace game{

	class Camera;
	class Level;
	class TextLabel;
	class UserInterface;
    class InputSystem;
    class FpsCounter;

    class GameState: public EventSupportableImpl {
    public:
        GameState(const GameStateType* stateData);

        void drawFrame();
        
        Level* getLevel();
        std::string getStateName();

    private:
        FpsCounter fpsCounter;
        Logger logger;
        std::string stateName;
        std::unique_ptr<Level> level;

        TextLabel* fpsText;
    };
    
    
}
#endif

