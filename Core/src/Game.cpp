#include <ChibiEngine/Game.h>
#include <ChibiEngine/Render/Particles/ParticleSystem.h>
#include <ChibiEngine/Render/Particles/EffectTempl.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Physics/Physics.h>
#include <ChibiEngine/SoundSystem/SoundSystem.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/Clock/ClockSystem.h>
#include <ChibiEngine/Script/Script.h>
#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/GameState/Level.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/XMLParser/SettingsParser.h>
#include <ChibiEngine/Common/ParserUtils.h>



using namespace game;
using namespace std;
using namespace rapidxml;

Game *Game::instance = nullptr;
static string LOGGER_NAME = "GameLogger";
const string Game::DEFAULT_CONFIG_FILE = "settings.xml";

void Game::initCoreSystems(std::string configFile, const char *pathPrefix) {
    FileSystem::pathPrefix = pathPrefix;
    instance = new Game();

    instance->logger.debug("Begin init FileSystem");
    instance->fileSystem = make_unique<FileSystem>(configFile);
    instance->clockSystem = make_unique<ClockSystem>();
    {

        instance->settings = make_unique<SettingsParserRoot>(createRoot<SettingsParserRoot>(configFile));
        instance->configPath = FileSystem::getPath(configFile);
        {
            PropertiesList *pL = instance->settings->settings->properties;
            if (pL!=nullptr){
                for (const auto& iter : pL->property) {
                    instance->properties[iter->name]=iter->value;
                }
            }
        }
    }
}

void Game::initGraphSystems() {
    instance->shaderSystem = make_unique<ShaderSystem>(getConfigPath()+"/"+instance->settings->settings->screen->shaders);
    instance->screenSystem = make_unique<ScreenSystem>(instance->settings->settings->screen);
    instance->camera = make_unique<Camera>(instance->settings->settings->camera);
}

ProfileType *Game::getCurrentInputProfile() {
    InputConfigType *ic = settings->settings->inputConfig;
    if (ic == nullptr)
        return nullptr;

    for (auto iter : ic->profile) {
        if (iter->name == ic->activeProfile)
            return iter;
    }

    logger.error("Current Input profile not found: " + ic->activeProfile);
    return nullptr;
}

GameStateType *Game::findGameStateData(const string &name) {
    GameStateType *gameStateData = nullptr;
    for (auto iter : settings->settings->gameStates->state) {
        if (iter->name == name) {
            gameStateData = iter;
        }
    }

    if (gameStateData == nullptr)
        logger.error("Initial game state not found :" + name);

    return gameStateData;
}

void Game::initGameState() {
    const string &initial = settings->settings->gameStates->initial;

    eventSystem = make_unique<EventSystem>();
    inputSystem = make_unique<InputSystem>(instance->getCurrentInputProfile());
    ui = make_unique<UserInterface>(getInputSystem(), getEventSystem());
    scriptSystem = make_unique<ScriptSystem>();
    newGameState = initial;
    need2SwitchGameState = true;
}

Game::Game() :
        loggingSystem(make_unique<LoggingSystem>()),
        logger(loggingSystem->createLogger(LOGGER_NAME)),
        levelClock(nullptr){
}

void Game::dispose() {
    delete instance;
}

std::string Game::getProperty(const std::string& name){
    return instance->properties[name];
}

ScreenType* Game::getScreenSettings(){
    return instance->settings->settings->screen;
}

void Game::switchGameState(const std::string &stateName) {
    instance->newGameState = stateName;
    instance->need2SwitchGameState = true;
}

std::string Game::getConfigPath(){
    return instance->configPath;
}

void Game::switchGameState() {
    GameStateType *data = findGameStateData(newGameState);
    if (data != nullptr) {
        if(levelClock)
            levelClock->reset();
        else
            levelClock=Game::getClockSystem()->createClock(0);

        gameState.reset(nullptr);
        eventSystem.reset(new EventSystem());
        inputSystem.reset(new InputSystem(instance->getCurrentInputProfile()));
        scriptSystem.reset(new ScriptSystem());
        ui.reset(new UserInterface(getInputSystem(), getEventSystem()));
        resources.reset(new Resources(settings->settings->resources));
        particleSystem.reset(new ParticleSystem());
        soundSystem.reset(new SoundSystem());
        physicsSystem.reset(new PhysicsSystem(settings->settings->physics, getLevelClock(), getEventSystem()));
        Game::getUserInterface()->loadFromFile(getConfigPath()+"/"+settings->settings->gameStates->userInterface);
        gameState.reset(new GameState(data));
    }
    need2SwitchGameState = false;
}



void Game::setLevelFactory(const LevelFactory& factory) {
    instance->levelFactory=factory;
    instance->initGameState();
}

const LookAndFeel& Game::getLookAndFeel() {
    return getScreen()->getLookAndFeel();
}


const PrimitiveDrawer *Game::getPrimitiveDrawer() {
    return getUserInterface()->getPrimitiveDrawer();
}

void Game::gameStep() {
    if (instance->need2SwitchGameState) {
        instance->switchGameState();
    }

    if (instance->gameState != 0x0) {
        Game::getEventSystem()->processEvents();
        instance->gameState->drawFrame();

    }
}
