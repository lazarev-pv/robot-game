#ifndef __H_ROBOT_SCRIPTS_
#define __H_ROBOT_SCRIPTS_

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <RobotLevel.h>
#include <ChibiEngine/Script/Script.h>
#include <ChibiEngine/Log/Logger.h>
#include <LightSource.h>
#include <LevelObjects/Interactive.h>
#include <LevelObjects/FXObject.h>
#include <Layer.h>
#include <LevelObjects/Box.h>

using namespace std;
using namespace game;
using namespace glm;

//object_setVelocity <name> <x> <y>


static int object_color(lua_State* lua){
    lua_settop(lua,5);
    LevelObject* obj = RobotLevel::instance()->findObject(lua_tostring(lua, 1));
    vec4 color(lua_tonumber(lua, 2),
            lua_tonumber(lua, 3),
            lua_tonumber(lua, 4),
            lua_tonumber(lua, 5));
    if(obj){
        obj->setColor(color);
    }

    return 0;
}

static int object_setVelocity(lua_State* lua){
    lua_settop(lua,3);
    PhysLevelObject* obj =dynamic_cast<PhysLevelObject*>(
            RobotLevel::instance()->findObject(lua_tostring(lua, 1)));

    obj->getBody()->SetLinearVelocity(
            b2Vec2(lua_tonumber(lua, 2),lua_tonumber(lua, 3)));

    return 0;
}


static int setObjectEnable(lua_State* lua){
    lua_settop(lua,2);
    string name(lua_tostring(lua, 1));

    LevelObject* lo = RobotLevel::instance()->findObject(name);
    if(lo!=nullptr){
        lo->setEnabled(lua_toboolean(lua,2));
        return 0;
    }

    RobotLevel::instance()->getLogger().error("No object [%s] found ",name.c_str());
    return 0;
}

static int addCharAnimation(lua_State *lua){
    lua_settop(lua,2);
    string anim(lua_tostring(lua, 1));
    RobotLevel::instance()->getCharacter()->getSkeleton()->addAnimation(anim,lua_toboolean(lua, 2));
    return 0;
}

static int removeCharAnimation(lua_State *lua){
    lua_settop(lua,1);
    string anim(lua_tostring(lua, 1));
    RobotLevel::instance()->getCharacter()->getSkeleton()->removeAnimation(anim);
    return 0;
}


static int setCharSlotImg(lua_State* lua){
    lua_settop(lua,2);
    string name(lua_tostring(lua, 1));
    string pic(lua_tostring(lua, 2));
    RobotLevel::instance()->getCharacter()->getSkeleton()->
            changeSlotImage(name, pic);
    return 0;
}

static int hit(lua_State* lua){
    lua_settop(lua,1);
    RobotLevel::instance()->getCharacter()->hitted(lua_tonumber(lua, 1));
    return 0;
}

struct ScriptAnimationListener{

    ScriptAnimationListener(lua_State* lua, const std::string& script):
            lua(lua),script(script){};

    void operator()(const AnimationEvent& event){
        lua_getglobal(lua,script.c_str());
        lua_pushstring(lua,event.eventName.c_str());

        if(lua_pcall(lua,1,0,0)!=LUA_OK){
            Game::getScriptSystem()->getLogger().error(string(lua_tostring(lua,-1)));
        }
    }

    lua_State* lua;
    string script;
};

static int addCharAnimationListener(lua_State* lua){
    lua_settop(lua,1);
    int id = RobotLevel::instance()->getCharacter()->getSkeleton()->
            addEventListeners(ScriptAnimationListener(lua,string(lua_tostring(lua, 1))));
    lua_pushinteger(lua, id);
    return 1;
}


struct RemoveAnimEvent {
    RemoveAnimEvent(int id):id(id){};
    void operator()(){
        RobotLevel::instance()->getCharacter()->getSkeleton()->
                removeEventListeners(id);
    }
    int id;
};

static int removeCharAnimationListener(lua_State* lua){
    lua_settop(lua,1);
    Game::getEventSystem()->doNextFrame(RemoveAnimEvent(lua_tointeger(lua, 1)), 1);
    return 0;
}


static int blockInput(lua_State* lua){
    lua_settop(lua,1);
    RobotLevel::instance()->getInputListener()->setBlockInput(lua_toboolean(lua, 1));
    return 0;
}

static int setCameraVelocity(lua_State* lua){
    lua_settop(lua,3);
    float x = lua_tonumber(lua, 1);
    float y = lua_tonumber(lua, 2);
    float z = lua_tonumber(lua, 3);
    RobotLevel::camera()->setVelocity(vec3(x,y,z));
    return 0;
}

static int setCameraFollowChar(lua_State* lua){
    lua_settop(lua,3);
    LevelObject* obj = RobotLevel::instance()->findObject(lua_tointeger(lua,1));
    vec2 v(lua_tonumber(lua, 2),lua_tonumber(lua, 3));
    RobotLevel::camera()->setHook(obj,v);
    return 0;
}

static int setDeleteObject(lua_State* lua){
    lua_settop(lua,1);
    RobotLevel::instance()->deleteObject(lua_tostring(lua,1));
    return 0;
}

//name scipt w h
static int makeObjectInteractive(lua_State* lua){
    lua_settop(lua,4);
    RobotLevel* level = RobotLevel::instance();
    LevelObject* plo = level->findObject(string (lua_tostring(lua,1)));
    new Interactive(level->generateUid(),plo,vec2(lua_tonumber(lua,3),lua_tonumber(lua,4)),string (lua_tostring(lua,2)));
    //plo->getLayer()->add(inter);
    return 0;
}

static int breakBody(lua_State* lua){
    lua_settop(lua,1);
    RobotLevel* level = RobotLevel::instance();
    Box* plo = dynamic_cast<Box*>(level->findObject(string (lua_tostring(lua,1))));
    if(plo==nullptr){
        level->getLogger().error("Box to break %s not found",lua_tostring(lua,1));
        return 0;
    }
    plo->breakBody();
    return 0;
}

static int setDayTime(lua_State* lua){
    lua_settop(lua,1);
    RobotLevel::instance()->setDayTime(lua_tonumber(lua,1));
    return 0;
}

static int camera_easePow(lua_State* lua) {
    lua_settop(lua,1);
    RobotLevel::camera()->setEasingPow(lua_tointeger(lua, 1));
    return 0;
}
/*
effect_play
    <effectName>
    <vec4(parms)>
    <rgba(color)>
    <zOrder=0>
    <vec2(dir)=(0,0)>
*/
static int effect_play(lua_State* lua){
    unsigned int n = lua_gettop(lua);
    assert(n>=3);

    string effectName(lua_tostring(lua, 1));
    vec3 pos(getFloat(lua, 2, "x"),getFloat(lua, 2, "y"),0.0);
    vec2 size(getFloat(lua, 2, "hw"),getFloat(lua, 2, "hh"));
    Color color(getColor(lua,3));
    float zOrder = n>=4? lua_tointeger(lua, 4):0;
    vec2 dir = n>=5? vec2(getFloat(lua, 5, "x"),getFloat(lua, 5, "y")):vec2(0,0);

    RobotLevel* level = RobotLevel::instance();
    Layer* layer = level->getMainLayer();
    FXObject* obj = new FXObject(
            level->generateUid(),
            layer,
            EffectParams(pos, size, dir, color),
            effectName, zOrder,""
    );
    layer->add(obj);

    // Через секунду объекта не будет
    Clock* c = Game::getLevelClock();
    Game::getEventSystem()->schedule(c, [layer, obj](){
        layer->deleteObject(obj);
    }, c->getTime()+1000, 0, 1);
    return 0;
}

void installScripts(ScriptSystem* ss){
    ss->addFunction("light_dayTime", setDayTime);
    ss->addFunction("char_addAnimation", addCharAnimation);
    ss->addFunction("char_removeAnimation", removeCharAnimation);
    ss->addFunction("char_slotImg", setCharSlotImg);
    ss->addFunction("char_addAnimationListener", addCharAnimationListener);
    ss->addFunction("char_removeAnimationListener", removeCharAnimationListener);
    ss->addFunction("char_hit", hit);
    ss->addFunction("camera_lock", setCameraFollowChar);
    ss->addFunction("camera_velocity", setCameraVelocity);
    ss->addFunction("camera_easePow", camera_easePow);
    ss->addFunction("input_block", blockInput);
    ss->addFunction("object_enable",setObjectEnable);
    ss->addFunction("object_delete", setDeleteObject);
    ss->addFunction("object_makeInteractive", makeObjectInteractive);
    ss->addFunction("object_break", breakBody);
    ss->addFunction("object_setVelocity", object_setVelocity);
    ss->addFunction("object_color",object_color);
    ss->addFunction("effect_play",effect_play);
}


#endif