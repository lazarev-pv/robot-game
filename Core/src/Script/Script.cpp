#include <ChibiEngine/Script/Script.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/SoundSystem/SoundSystem.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Clock/EventSystem.h>
#include <ChibiEngine/Clock/ClockSystem.h>
#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/Input/Input.h>
#include <ChibiEngine/Render/Camera.h>
#include <assert.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Common/MathUtils.h>

#include <glm/glm.hpp>
#include "ScriptsEventListener.hpp"
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

using namespace game;
using namespace std;
using namespace glm;

static int moveCamera(lua_State* lua){
    lua_settop(lua,3);
    float x = lua_tonumber(lua, 1);
    float y = lua_tonumber(lua, 2);
    float z = lua_tonumber(lua, 3);
    Game::getCamera()->moveCamera(vec3(x,y,z));
    return 0;
}

static int setCameraPosition(lua_State* lua){
    lua_settop(lua,3);
    float x = lua_tonumber(lua, 1);
    float y = lua_tonumber(lua, 2);
    float z = lua_tonumber(lua, 3);
    Game::getCamera()->setPosition(vec3(x,y,z));
    return 0;
}

static int getCameraPosition(lua_State* lua){
    lua_settop(lua,0);
    vec3 p = Game::getCamera()->getPosition();
    lua_pushnumber(lua, p.x);
    lua_pushnumber(lua, p.y);
    lua_pushnumber(lua, p.z);
    return 3;
}

static int createMusic(lua_State* lua){
    unsigned int n = lua_gettop(lua);
    assert(n>=1);

    const char* name = lua_tostring(lua, 1);
    bool infinite = (n>1)?lua_toboolean(lua, 2):true;
    string path = Game::getResources()->findResourcePath(string(name));

    int id = Game::getSoundSystem()->createMusic(path,infinite);
    lua_pushinteger(lua, id);
    return 1;
}

static int playMusic(lua_State* lua){
    int id = lua_tointeger(lua, 1);
    Game::getSoundSystem()->getMusic(id).play();
    return 0;
}

static int createSound(lua_State* lua){
    const char* name = lua_tostring(lua, 1);
    string path = Game::getResources()->findResourcePath(string(name));
    int id = Game::getSoundSystem()->createSound(path);
    lua_pushinteger(lua, id);
    return 1;
}

static int playSound(lua_State* lua){
    int id = lua_tointeger(lua, 1);
    Game::getSoundSystem()->getSound(id).play();
    return 0;
}

static int sound_stopAll(lua_State* lua){
    lua_settop(lua,0);
    Game::getSoundSystem()->stopAllSounds();
    return 0;
}


static int clock_pause(lua_State* lua){
    lua_settop(lua,1);
    Game::getLevelClock()->pause(lua_toboolean(lua,1));
    return 0;
}


static int game_loadState(lua_State* lua){
    lua_settop(lua,1);
    Game::switchGameState(string(lua_tostring(lua, 1)));
    return 0;
}


static int clock_scale(lua_State* lua){
    lua_settop(lua,1);
    Game::getLevelClock()->scaleClock(lua_tonumber(lua, 1));
    return 0;
}

//<float(targetScale)> <long(duration|ms)> <string(type|"in","out","inout")>
constexpr int SOOTH_CHANGE_INTERVAL = 20;
static int clock_smoothScale(lua_State* lua) {
    lua_settop(lua, 4);
    float scale = lua_tonumber(lua, 1);
    long duration = lua_tointeger(lua, 2);
    string type(lua_tostring(lua, 3));
    double pow = lua_tonumber(lua, 4);

    std::function<double(double,double,double)> easeFactorFunc =
            type=="in" ?game::easeIn:
            type=="out"?game::easeOut:
                        game::easeInOut;


    int passedTime = 0;
    float beginScale = Game::getLevelClock()->getScaleFactor();
    float deltaScale = scale - beginScale;
  //  Game::getLogger().error("Smooth begin [%f] delta [%f] dur [%f]  pow [%f]",begin,deltaScale, duration,pow);

    Game::getEventSystem()->schedule(Game::getClockSystem()->getImmutableClock(),
            [passedTime,easeFactorFunc,beginScale, deltaScale, duration, pow]() mutable{
                Game::getLevelClock()->scaleClock(beginScale + deltaScale*easeFactorFunc(passedTime,duration,pow));
                passedTime+=SOOTH_CHANGE_INTERVAL;
            }, Game::getClockSystem()->getImmutableClock()->getTime(),SOOTH_CHANGE_INTERVAL, duration/SOOTH_CHANGE_INTERVAL);
    return 0;
}

class FunctionExecuter{
public:
    FunctionExecuter(string &funcName, string& command) :
            funcName(move(funcName)),
            command(move(command))
    {}
    void operator()(){
        if(funcName.size()>0)
            Game::getScriptSystem()->doScript(funcName);
        if(command.size()>0)
            Game::getScriptSystem()->doString(command);
    }
private:
    string funcName;
    string command;
};

static int clock_postPhysStep(lua_State* lua){
    lua_settop(lua,1);
    lua_getfield(lua,1,"command");
    string command(luaL_optstring(lua, -1, ""));
    lua_getfield(lua,1,"func");
    string func(luaL_optstring(lua, -1, ""));

    if(command.size()>0){
        Game::getPhysicsSystem()->addPostStepAction([command](){
            Game::getScriptSystem()->doString(command);
        });
    }

    if(func.size()>0){
        Game::getPhysicsSystem()->addPostStepAction([func](){
            Game::getScriptSystem()->doScript(func);
        });
    }

    return 0;
}


float game::getFloat(lua_State* lua, int parmIndex, const char* name){
    lua_getfield(lua,parmIndex,name);
    float r = static_cast<float>(luaL_optnumber(lua,-1,0.0));
    lua_pop(lua,1);
    return r;
}

Color game::getColor(lua_State* lua, int parmIndex){
    return Color(
            getFloat(lua, parmIndex, "r"),
            getFloat(lua, parmIndex, "g"),
            getFloat(lua, parmIndex, "b"),
            getFloat(lua, parmIndex, "a")
    );
}

static int schedule(lua_State* lua){
    lua_settop(lua,1);

    lua_getfield(lua,1,"func");
    string funcName(luaL_optstring(lua, -1,""));

    lua_getfield(lua,1,"command");
    string commandName(luaL_optstring(lua, -1,""));

    lua_getfield(lua,1,"startOffset");
    int startOffset = luaL_optinteger(lua, -1, 0);

    lua_getfield(lua,1,"infinite");
    bool infinite = lua_toboolean(lua,-1);

    lua_getfield(lua,1,"repeats");
    int repeats = luaL_optinteger(lua,-1,1);

    lua_getfield(lua,1,"interval");
    int interval = luaL_optinteger(lua,-1,1000);

    lua_pop(lua,5);
    ClockHandle clock = Game::getLevelClock();
    if(infinite){
        Game::getEventSystem()->scheduleInfiteRepeat(
                clock, FunctionExecuter(funcName, commandName), clock->getTime() + startOffset, interval);
    }else{
        Game::getEventSystem()->schedule(
                clock, FunctionExecuter(funcName, commandName), clock->getTime() + startOffset, interval, repeats);
    }
    return 0;
}


ScriptSystem::ScriptSystem():
        lua(luaL_newstate()),
        logger(Game::getLoggingSystem()->createLogger("ScriptSystem")),
        loadedFiles(),
        listener(make_unique<ScriptsEventListener>(lua)){

    Game::getInputSystem()->addRawListener(getListener());
    luaL_openlibs(lua);

    vector<pair<const char*,int(*)(lua_State *)>> funcs({
            {"music_create",createMusic},
            {"music_play", playMusic},
            {"sound_create",createSound},
            {"sound_play", playSound},
            {"sound_stopAll", sound_stopAll },
            {"clock_schedule",schedule},
            {"clock_postPhysStep",clock_postPhysStep},
            {"camera_move",moveCamera},
            {"camera_setPosition",setCameraPosition},
            {"camera_getPosition",getCameraPosition},
            {"game_loadState",game_loadState},
            {"clock_smoothScale",clock_smoothScale},
            {"clock_scale",clock_scale},
            {"clock_pause",clock_pause}
    });

    for(auto iter : funcs){
        lua_pushcfunction(lua, iter.second);
        lua_setglobal(lua,iter.first);
    }
    addEventListenerScripts(this);
}

ScriptSystem::~ScriptSystem(){
    lua_close(lua);
}

void ScriptSystem::loadFile(const std::string& fileName){
    if(fileName.size()<=0)
        return;

    string fullName = Game::getFileSystem()->getFullName(fileName);

    logger.debug("loading script " + fullName);
    if(loadedFiles.find(fullName)!=loadedFiles.end()){
        logger.warn("File already loaded " + fullName);
        return;
    }

    if(luaL_loadfile(lua, fullName.c_str())!=LUA_OK){
        const char* msg = lua_tostring(lua, -1);
        logger.error("Load file failed: %s", msg);
        lua_pop(lua,1);
    }

    if(lua_pcall(lua,0,0,0)!=LUA_OK){
        const char* msg = lua_tostring(lua, -1);
        logger.error("Executing file failed: %s", msg);
        lua_pop(lua,1);
    }

    loadedFiles.insert(fullName);
}

void ScriptSystem::doScript(const std::string& functionName){
    if(functionName.size()<=0)
        return;
    lua_getglobal(lua,functionName.c_str());
    if(lua_pcall(lua,0,0,0)!= LUA_OK){
        const char* msg = lua_tostring(lua, -1);
        logger.error("Executing file failed: %s", msg);
        lua_pop(lua,1);
    }
}

void ScriptSystem::doString(const std::string& expression){
    if(expression.size()<=0)
        return;
    int error = luaL_loadstring(lua, expression.c_str()) || lua_pcall(lua, 0, 0, 0);
    if(error){
        logger.error("error while evaluating expression '%s': %s", expression.c_str(), lua_tostring(lua, -1));
        lua_pop(lua,1);
    }
}

void ScriptSystem::addFunction(const std::string& name, int(*func)(lua_State *)){
    lua_pushcfunction(lua, func);
    lua_setglobal(lua,name.c_str());
}

// ------------------------- GENERIC CALL -----------------------------


class StringArg {
public:
    StringArg(string val):val(val){}
    void operator()(lua_State* lua){
        lua_pushstring(lua,val.c_str());
    }
private:
    string val;
};

class IntArg {
public:
    IntArg(int val):val(val){}
    void operator()(lua_State* lua){
        lua_pushinteger(lua,val);
    }
private:
    int val;
};

const std::function<void(lua_State*)> game::arg(const std::string& val){
    return StringArg(val);
}
const std::function<void(lua_State*)> game::arg(int val){
    return IntArg(val);
}

void ScriptSystem::doScript(const std::string& funcName, const std::vector<std::function<void(lua_State*)>> args){
    lua_getglobal(lua,funcName.c_str());
    for(auto i = args.begin();i!=args.end();++i){
        (*i)(lua);
    }

    if(lua_pcall(lua,args.size(),0,0)!=LUA_OK){
        Game::getScriptSystem()->getLogger().error(string(lua_tostring(lua, -1)));
    }
}