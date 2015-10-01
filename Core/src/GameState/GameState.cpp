#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/GameState/Level.h>

#include <ChibiEngine/Render/Particles/ParticleSystem.h>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Log/LoggingSystem.h>

#include <ChibiEngine/UserInterface/TextLabel.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Clock/ClockSystem.h>

#include <sstream>
#include <iomanip>

using namespace glm;
using namespace game;
using namespace std;

const static string LOGGER_NAME = "GameState";

std::string GameState::getStateName(){
	return stateName;
}

GameState::GameState(const GameStateType* stateData):
	logger(Game::getLoggingSystem()->createLogger(LOGGER_NAME)),
	stateName(stateData->name),
	level(Game::getLevelFactory()(Game::getConfigPath()+"/"+stateData->levelDescription)){

	fpsText = new TextLabel("FPS:");
	fpsText->setPosition(vec3(0,1-fpsText->getHH(),1));
	Game::getUserInterface()->add(fpsText);
}

Level* GameState::getLevel(){
	return level.get();
}

void GameState::drawFrame(){
	CameraCHandle cam = Game::getCamera();
	double currFps = 1.0/(static_cast<double>(Game::getLevelClock()->getTimeDelta())*0.001);
	fpsCounter.add(currFps);
	if(fpsCounter.isReady2Update()){
		ostringstream _fps_text;
		_fps_text<<std::setprecision(5);
		_fps_text<<"FPS: "<<fpsCounter.calcAverageFps()
				<<std::setprecision(2)
				<<" Time:"<< Game::getLevelClock()->getScaleFactor();
		fpsText->setValue(_fps_text.str());
	}
    Game::getScreen()->Clear();
	level->draw(cam);
    Game::getUserInterface()->draw();
}
