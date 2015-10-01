#ifndef __HEADER_SRIPTS_EVENT_LISTENERS_
#define __HEADER_SRIPTS_EVENT_LISTENERS_

#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Script/Script.h>
#include <ChibiEngine/Clock/EventSystem.h>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

using namespace game;
using namespace std;

class ScriptsEventListener : public RawEventListener {
public:
    ScriptsEventListener(lua_State* lua):lua(lua){};

    bool onClickDown(MouseButton code, const glm::vec2 &pnt) override{
        for(auto iter : clickDownScripts){
            lua_getglobal(lua,iter.c_str());
            lua_pushnumber(lua,static_cast<int>(code));
            lua_pushnumber(lua,pnt.x);
            lua_pushnumber(lua,pnt.y);

            if(lua_pcall(lua,3,0,0)!=LUA_OK){
                Game::getScriptSystem()->getLogger().error(string(lua_tostring(lua,-1)));
            }
        }
        return false;
    }

    bool onClickUp(MouseButton code, const glm::vec2 &pnt) override{
        for(auto iter : clickUpScripts){
            lua_getglobal(lua,iter.c_str());
            lua_pushnumber(lua,static_cast<int>(code));
            lua_pushnumber(lua,pnt.x);
            lua_pushnumber(lua,pnt.y);

            if(lua_pcall(lua,3,0,0)!=LUA_OK){
                Game::getScriptSystem()->getLogger().error(string(lua_tostring(lua,-1)));
            }
        }
        return false;
    }

    bool onMouseMove(const glm::vec2 &pnt) override{
        for(auto iter : mouseMoveScripts){
            lua_getglobal(lua,iter.c_str());
            lua_pushnumber(lua,pnt.x);
            lua_pushnumber(lua,pnt.y);

            if(lua_pcall(lua,2,0,0)!=LUA_OK){
                Game::getScriptSystem()->getLogger().error(string(lua_tostring(lua,-1)));
            }
        }
        return false;
    }

    bool onKeyPressed(KeyCode keyCode, InputEvent::State state) override{
        for(auto iter : keyPressedScripts){
            lua_getglobal(lua,iter.c_str());
            lua_pushunsigned(lua,keyCode);
            lua_pushboolean(lua,state==InputEvent::PRESSED);

            if(lua_pcall(lua,2,0,0)!=LUA_OK){
                Game::getScriptSystem()->getLogger().error(string(lua_tostring(lua,-1)));
            }
        }
        return false;
    }

    bool onSpecialKeyPressed(int keyCode, InputEvent::State state) override{
        for(auto iter : specialKeyPressedScripts){
            lua_getglobal(lua,iter.c_str());
            lua_pushunsigned(lua,keyCode);
            lua_pushboolean(lua,state==InputEvent::PRESSED);

            if(lua_pcall(lua,2,0,0)!=LUA_OK){
                Game::getScriptSystem()->getLogger().error(string(lua_tostring(lua,-1)));
            }
        }
        return false;
    }

    std::vector <string> clickDownScripts;
    std::vector <string> clickUpScripts;
    std::vector <string> mouseMoveScripts;
    std::vector <string> keyPressedScripts;
    std::vector <string> specialKeyPressedScripts;
private:
    lua_State* lua;
};

static int addClickDownScripts(lua_State* lua){
    lua_settop(lua,1);
    Game::getScriptSystem()->getListener()->
            clickDownScripts.push_back(string(lua_tostring(lua, 1)));
    return 0;
}

static int addClickUpScripts(lua_State* lua){
    lua_settop(lua,1);
    Game::getScriptSystem()->getListener()->
            clickUpScripts.push_back(string(lua_tostring(lua, 1)));
    return 0;
}

static int addMouseMoveScripts(lua_State* lua){
    lua_settop(lua,1);
    Game::getScriptSystem()->getListener()->
            mouseMoveScripts.push_back(string(lua_tostring(lua, 1)));
    return 0;
}

static int addKeyPressedScripts(lua_State* lua){
    lua_settop(lua,1);
    Game::getScriptSystem()->getListener()->
            keyPressedScripts.push_back(string(lua_tostring(lua, 1)));
    return 0;
}

static int addSpecialKeyPressedScripts(lua_State* lua){
    lua_settop(lua,1);
    Game::getScriptSystem()->getListener()->
            specialKeyPressedScripts.push_back(string(lua_tostring(lua, 1)));
    return 0;
}

struct Remove{
    Remove(std::vector <string>* cont, const string& name):
            name(name),cont(cont){};
    void operator()(){
        remove(cont, name);
    }
    string name;
    std::vector <string>* cont;
};

static int removeClickDownScripts(lua_State* lua){
    lua_settop(lua,1);
    Game::getEventSystem()->doNextFrame(Remove(
            &Game::getScriptSystem()->getListener()->clickDownScripts,
            string(lua_tostring(lua, 1))));
    return 0;
}

static int removeClickUpScripts(lua_State* lua){
    lua_settop(lua,1);
    Game::getEventSystem()->doNextFrame(Remove(
            &Game::getScriptSystem()->getListener()->clickUpScripts,
            string(lua_tostring(lua, 1))));
    return 0;
}

static int removeMouseMoveScripts(lua_State* lua){
    lua_settop(lua,1);
    Game::getEventSystem()->doNextFrame(Remove(
            &Game::getScriptSystem()->getListener()->mouseMoveScripts,
            string(lua_tostring(lua, 1))));
    return 0;
}

static int removeKeyPressedScripts(lua_State* lua){
    lua_settop(lua,1);
    Game::getEventSystem()->doNextFrame(Remove(
            &Game::getScriptSystem()->getListener()->keyPressedScripts,
            string(lua_tostring(lua, 1))));
    return 0;
}

static int removeSpecialKeyPressedScripts(lua_State* lua){
    lua_settop(lua,1);
    Game::getEventSystem()->doNextFrame(Remove(
            &Game::getScriptSystem()->getListener()->specialKeyPressedScripts,
            string(lua_tostring(lua, 1))));
    return 0;
}

static void addEventListenerScripts(ScriptSystem* ss){
    ss->addFunction("input_addClickDown", addClickDownScripts);
    ss->addFunction("input_addClickUp", addClickUpScripts);
    ss->addFunction("input_addMouseMove", addMouseMoveScripts);
    ss->addFunction("input_addKeyPressed", addKeyPressedScripts);
    ss->addFunction("input_addSpecKeyPressed", addSpecialKeyPressedScripts);

    ss->addFunction("input_removeClickDown", removeClickDownScripts);
    ss->addFunction("input_removeClickUp", removeClickUpScripts);
    ss->addFunction("input_removeMouseMove", removeMouseMoveScripts);
    ss->addFunction("input_removeKeyPressed", removeKeyPressedScripts);
    ss->addFunction("input_removeSpecKeyPressed", removeSpecialKeyPressedScripts);
}

#endif