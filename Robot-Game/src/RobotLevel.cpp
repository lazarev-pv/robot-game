#include <ChibiEngine/GameState/Level.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/SoundSystem/SoundSystem.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Script/Script.h>
#include <LevelObjects/EventObject.h>
#include <ChibiEngine/Physics/CategoryReference.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/Common/ParserUtils.h>

#include <RobotLevel.h>
#include <Layer.h>
#include <Character.h>
#include <AICharacter.h>
#include <LevelObjects/Interactive.h>
#include <LightSource.h>
#include <unordered_map>
#include <CameraController.h>
#include "RobotScripts.hpp"
#include "AIScripts.hpp"
#include "Generated/RobotConfigParser.h"
#include <LevelObjects/PointObject.h>


using namespace std;
using namespace game;
using namespace glm;
using namespace rapidxml;

RobotLevel* RobotLevel::instance(){
    return static_cast<RobotLevel*>(Game::getGame()->getLevel());
}

CameraController* RobotLevel::camera(){
    return instance()->cameraController;
}
CharacterReference* RobotLevel::chars(){
    return instance()->charReference;
}




struct DoScriptNextFrame{
    DoScriptNextFrame(string script):script(script){};
    void operator()(){
        Game::getScriptSystem()->doScript(script);
    }
    string script;
};

void RobotLevel::load(const std::string& worldFile) {

    auto root(createRoot<WorldParserRoot>(worldFile));

    Game::getResources()->load(root.world->resources, worldFile);

    //////
        string configFile = Game::getConfigPath() + "/" + Game::getProperty("settings");
        auto root2(createRoot<RobotConfigParserRoot>(configFile));
        charReference = new CharacterReference(root2.config,FileSystem::getPath(configFile));
    /////

    for(auto layer : root.world->layer){
        layers.push_back(new Layer(layer,this));
    }

    sort(layers.begin(),layers.end(),[](const Layer* l1, const Layer* l2){
        return l1->getZ()<l2->getZ();
    });

    for(auto iter : layers){
        if(iter->getName()=="main"){
            mainLayer =iter;
            break;
        }
    }
    assert(mainLayer != nullptr);

    glm::vec2 startPoint;
	for(auto iter : root.world->points){
		if(iter->name=="begin"){
			startPoint=vec2(iter->x,iter->y);
		}
        mainLayer->add(new PointObject(
                vec3(iter->x,iter->y,0),
                generateUid(),
                mainLayer,
                iter->name));
	}

	character = charReference->createChar(generateUid(),"char", startPoint,
            CategoryReference::get(CategoryReference::PROTAGONIST),mainLayer);
    mainLayer->add(add2Index(character));
	if(root.world->enemies!=nullptr){
		for(auto iter: root.world->enemies->enemy){
			Character* en =  charReference->createEnemy(generateUid(),iter->enemyClass, vec2(iter->pos->x,iter->pos->y),mainLayer);
           mainLayer->add(add2Index(en));
		}
	}

    //todo сделать это для всех слоев
    mainLayer->resort();
	{
        this->dayLight=Color(parse(root.world->weather->dayLight));
        this->nightLight=Color(parse(root.world->weather->nightLight));
        setDayTime(root.world->weather->daytime);

	}

    cameraController = new CameraController(root.world->camera,root2.config->camera);

    { // init scripts
        LevelSettings* ls = root.world->levelSettings;
        if(ls!=nullptr){
            installScripts(Game::getScriptSystem());
            installAIScripts(Game::getScriptSystem());
            Game::getScriptSystem()->loadFile(FileSystem::getPath(worldFile)+"/"+ls->scriptName);
            Game::getEventSystem()->doNextFrame(DoScriptNextFrame(ls->startEvent), 1);
        }
    }

    for(auto iter : root.world->event){
        mainLayer->add(add2Index(new EventObject(generateUid(),iter,mainLayer)));
    }
}

void RobotLevel::setDayTime(float k){
    ambientLight=dayLight*k+nightLight*(1.0f-k);
}


RobotLevel::RobotLevel(const std::string& worldFile)
	:currentInteractive(nullptr),
     logger(Game::getLoggingSystem()->createLogger("RobotLevel")),
     moveListener(this),
	 drawDebug(false),drawPres(true),
	 ambientLight(Color::White),
    lastUid(0){

	load(worldFile);
    Game::getPhysicsSystem()->setContactListener(&contactListener);
    Game::getInputSystem()->addEventListener(&moveListener);
}

RobotLevel::~RobotLevel(){
    deleteAll(layers);
    delete charReference;
}

void RobotLevel::toggleDebug(){
	drawDebug=!drawDebug;
	character->setDrawDebug(drawDebug);
    for (Layer *layer : layers) {
        layer->setDrawDebug(drawDebug);
    }
}

void RobotLevel::togglePres(){
    drawPres=!drawPres;
    character->setEnabled(drawPres);
    for (Layer *layer : layers) {
        layer->setDrawPres(drawPres);
    }
}

void RobotLevel::deleteObject(const std::string &name){
    LevelObject* lo = findObject(name);
    if(lo!=nullptr)
        lo->getLayer()->deleteObject(lo);
}

LevelObject* RobotLevel::findObject(const std::string &name){
    for(auto iter : layers){
        auto s = iter->findObject(name);
        if(s!=nullptr)
            return s;
    }
    return nullptr;
}

LevelObject *RobotLevel::findObject(int uid){
    return globalIndex[uid];
}


void RobotLevel::draw(CameraCHandle camera){
    for(auto l : layers){
        l->draw(ambientLight);
	}
}

void RobotLevel::viewInteractive(Interactive* current){
    currentInteractive = current;
    Game::getUserInterface()->findElement("interactive")->
            setVisible(current!=nullptr);
}

void RobotLevel::interact(){
    if(currentInteractive){
        currentInteractive->interact();
    }
}