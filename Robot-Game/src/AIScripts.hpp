#ifndef __H_AI_ROBOT_SCRIPTS_
#define __H_AI_ROBOT_SCRIPTS_

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <RobotLevel.h>
#include <ChibiEngine/Script/Script.h>
#include <ChibiEngine/Log/Logger.h>
#include <CharacterReference.h>
#include <Character.h>
#include <Layer.h>

using namespace std;
using namespace game;
using namespace glm;

static inline Character* getChar(int uid){
    Character* ch = static_cast<Character*>(RobotLevel::instance()->findObject(uid));
    if(ch==nullptr){
        RobotLevel::instance()->getLogger().error("Character with uid [%i] not found ",uid);
    }

    return ch;
}

static int ai_findByName(lua_State* lua){
    lua_settop(lua,1);
    string name(lua_tostring(lua, 1));
    LevelObject* o = RobotLevel::instance()->findObject(name);
    if(o==nullptr){
        RobotLevel::instance()->getLogger().error("findByName:  name %s not found",name.c_str());
        return 0;
    }

    lua_pushinteger(lua,o->getUid());
    return 1;
}



static int ai_spawn(lua_State* lua){
    lua_settop(lua,3);
    string name(lua_tostring(lua, 1));
    vec2 pos(lua_tonumber(lua,2),lua_tonumber(lua,3));

    RobotLevel* level = RobotLevel::instance();
    int uid = level->generateUid();

    Game::getPhysicsSystem()->addPostStepAction([level,uid,name,pos](){
        Layer* layer = level->getCharacter()->getLayer();
        layer->add(RobotLevel::chars()->createEnemy(uid,name,pos,layer));

    });
    lua_pushnumber(lua,uid);
    return 1;
}

static int ai_getPosition(lua_State* lua){
    lua_settop(lua,1);

    LevelObject* obj = RobotLevel::instance()->findObject(lua_tointeger(lua, 1));
    glm::vec3 v = obj->getPosition();
    lua_pushnumber(lua,v.x);
    lua_pushnumber(lua,v.y);
    return 2;
}

static int ai_move(lua_State* lua){
    lua_settop(lua,2);
    getChar(lua_tointeger(lua,1))->move(lua_toboolean(lua,2));
    return 0;
}

static int ai_attack(lua_State* lua){
    lua_settop(lua,3);
    getChar(lua_tointeger(lua,1))->attack(
            lua_toboolean(lua,2),
            lua_tointeger(lua,3));

    return 0;
}

static int ai_stop(lua_State* lua){
    lua_settop(lua,1);
   getChar(lua_tointeger(lua,1))->stop();
    return 0;
}

static int ai_getMaxHealth(lua_State* lua){
    lua_settop(lua,1);
   lua_pushnumber(lua,getChar(lua_tointeger(lua,1))->getMaxHealth());
    return 1;
}

static int ai_getHealth(lua_State* lua){
    lua_settop(lua,1);
   lua_pushnumber(lua,getChar(lua_tointeger(lua,1))->getHealth());
    return 1;
}

static int ai_getProtagonistUid(lua_State* lua){
    lua_settop(lua,0);
    lua_pushinteger(lua,RobotLevel::instance()->getCharacter()->getUid());
    return 1;
}

static int ai_setWeapon(lua_State* lua){
    lua_settop(lua,3);
    Character* ch = getChar(lua_tointeger(lua, 1));
    ch->setSlot(
            lua_tointeger(lua, 2),
            RobotLevel::chars()->createWeapon(lua_tostring(lua, 3), ch));
    return 0;
}

static int ai_onDestroyed(lua_State* lua){
    lua_settop(lua,2);
    int uid = lua_tointeger(lua,1);
    PhysLevelObject* lo = static_cast<PhysLevelObject*>(RobotLevel::instance()->findObject(uid));
    if(lo==nullptr){
        RobotLevel::instance()->getLogger().error("ai_onDestroyed:  uid %d not found",uid);
        return 0;
    }
    string funcName(lua_tostring(lua, 2));
    lo->addDestroyListener([funcName](){
        Game::getScriptSystem()->doScript(funcName);
    });
    return 0;
}

void installAIScripts(ScriptSystem* ss){
    ss->addFunction("ai_getPosition",ai_getPosition);
    ss->addFunction("ai_move",ai_move);
    ss->addFunction("ai_attack",ai_attack);
    ss->addFunction("ai_stop",ai_stop);
    ss->addFunction("ai_getHealth",ai_getHealth);
    ss->addFunction("ai_getMaxHealth",ai_getMaxHealth);
    ss->addFunction("ai_getProtagonistUid",ai_getProtagonistUid);
    ss->addFunction("ai_setWeapon",ai_setWeapon);
    ss->addFunction("ai_findByName",ai_findByName);
    ss->addFunction("ai_spawn",ai_spawn);
    ss->addFunction("ai_onDestroyed",ai_onDestroyed);
}

#endif