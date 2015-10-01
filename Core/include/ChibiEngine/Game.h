
#ifndef __CONTEXT_H
#define	__CONTEXT_H

#include <string>
#include <unordered_map>
#include <ChibiEngine/Log/Logger.h>
#include <memory>
#include <ChibiEngine/Handles.h>
#include <ChibiEngine/Common/ClassUtils.h>

struct GameStateType;
struct ProfileType;
class SettingsParserRoot;
class ScreenType;

namespace game{
    class GameState;
    class FileSystem;
    class ShaderSystem;
    class ScreenSystem;
    class EventSystem;
    class LoggingSystem;
    class Logger;
    class ClockSystem;
    class PhysicsSystem;
    class ScriptingSystem;
    class LookAndFeel;
    class PrimitiveDrawer;
    class Resources;
    class Camera;
    class UserInterface;
    class InputSystem;
    class ParticleSystem;
    class SoundSystem;
    class ScriptSystem;
    class Clock;
    class Level;

    class Game : private UniqueClass {
    public:
        static const std::string DEFAULT_CONFIG_FILE;
        using LevelFactory = std::function<std::unique_ptr<Level>(const std::string)>;

        // Initializers
        static void initCoreSystems(std::string configFile = DEFAULT_CONFIG_FILE, const char *pathPrefix = "");
        static void initGraphSystems();
        static void dispose();

        //Service Getters
        static const PrimitiveDrawer* getPrimitiveDrawer();
        static const LookAndFeel& getLookAndFeel();
        inline static const Logger& getLogger();
        inline static EventSystem* getEventSystem();
        inline static Resources* getResources();
        inline static ScreenSystem* getScreen();
        inline static FileSystem *getFileSystem();
        inline static LoggingSystem* getLoggingSystem();
        inline static CameraHandle getCamera();
        inline static GameState* getGame();
        inline static ShaderSystem* getShaderSystem();
        inline static UserInterface* getUserInterface();
        inline static InputSystem* getInputSystem();
        inline static ClockSystem* getClockSystem();
        inline static ParticleSystem* getParticleSystem();
        inline static SoundSystem* getSoundSystem();
        inline static ClockHandle getLevelClock();
        inline static ScriptSystem* getScriptSystem();
        inline static PhysicsSystem* getPhysicsSystem();

        //common operations
        static void switchGameState(const std::string& stateName);
        static void setLevelFactory(const LevelFactory& factory);
        static inline const LevelFactory& getLevelFactory();
        static void gameStep();

        static std::string getConfigPath();
        static std::string getProperty(const std::string& name);
        static ScreenType* getScreenSettings();

    private:
        Game();

        void switchGameState();
        GameStateType* findGameStateData(const std::string& gameStateName);
        ProfileType* getCurrentInputProfile();
        void initGameState();

        static Game* instance;

        std::unique_ptr<LoggingSystem> loggingSystem;
        Logger logger;
        std::unique_ptr<FileSystem> fileSystem;
        std::unique_ptr<ShaderSystem> shaderSystem;
        std::unique_ptr<ScreenSystem> screenSystem;
        std::unique_ptr<ClockSystem> clockSystem;
        std::unique_ptr<ParticleSystem> particleSystem;
        std::unique_ptr<SoundSystem> soundSystem;

        LevelFactory levelFactory;
        std::unique_ptr<UserInterface> ui;
        std::unique_ptr<EventSystem> eventSystem;
        std::unique_ptr<InputSystem> inputSystem;
        std::unique_ptr<ScriptSystem> scriptSystem;
        std::unique_ptr<Camera> camera;
        ClockHandle levelClock;
        std::unique_ptr<PhysicsSystem> physicsSystem;
        std::unique_ptr<Resources> resources;

        std::unique_ptr<GameState> gameState;
        bool need2SwitchGameState= false;
        std::string newGameState;

        std::unique_ptr<SettingsParserRoot> settings;
        std::string configPath;
        std::unordered_map<std::string, std::string> properties;
    };



    inline const Logger& Game::getLogger() {
        return instance->logger;
    }

    inline EventSystem *Game::getEventSystem() {
        return instance->eventSystem.get();
    }

    inline ParticleSystem *Game::getParticleSystem() {
        return instance->particleSystem.get();
    }

    inline SoundSystem *Game::getSoundSystem() {
        return instance->soundSystem.get();
    }

    inline ClockHandle Game::getLevelClock() {
        return instance->levelClock;
    }

    inline ScriptSystem *Game::getScriptSystem() {
        return instance->scriptSystem.get();
    }

    inline PhysicsSystem* Game::getPhysicsSystem(){
        return instance->physicsSystem.get();
    }

    inline FileSystem *Game::getFileSystem() {
        return instance->fileSystem.get();
    }

    inline ClockSystem *Game::getClockSystem() {
        return instance->clockSystem.get();
    }

    inline ShaderSystem *Game::getShaderSystem() {
        return instance->shaderSystem.get();
    }

    inline UserInterface *Game::getUserInterface() {
        return instance->ui.get();
    }

    inline InputSystem *Game::getInputSystem() {
        return instance->inputSystem.get();
    }

    inline LoggingSystem *Game::getLoggingSystem() {
        return instance->loggingSystem.get();
    }

    inline CameraHandle Game::getCamera() {
        return instance->camera.get();
    }

    inline const Game::LevelFactory& Game::getLevelFactory() {
        return instance->levelFactory;
    }

    inline Resources *Game::getResources() {
        return instance->resources.get();
    }

    inline ScreenSystem *Game::getScreen() {
        return instance->screenSystem.get();
    }

    inline GameState *Game::getGame() {
        return instance->gameState.get();
    }
}


#endif

