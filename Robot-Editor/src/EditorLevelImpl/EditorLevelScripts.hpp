#ifndef __H_EDITOR_SCRIPTS_
#define __H_EDITOR_SCRIPTS_


extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <EditorLevel.h>
#include <ChibiEngine/Script/Script.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/UserInterface/Checkbox.h>
#include <EditorLevelImpl/EditorConstants.h>

using namespace std;
using namespace game;
using namespace glm;

static int updateFilter(lua_State* lua){
    lua_settop(lua,1);
    string name(lua_tostring(lua, 1));

    bool val = Game::getUserInterface()->findCheckbox("select."+name)->isChecked();
    EditorLevel::instance()->setSelection(name,val);
    return 0;
}

static int updateAllFilter(lua_State* lua){
    lua_settop(lua,0);
    bool val = Game::getUserInterface()->findCheckbox("select.all")->isChecked();
    for(string name : EditorConstants::getTypes()){
        Game::getUserInterface()->findCheckbox("select."+name)->setChecked(val);
        EditorLevel::instance()->setSelection(name,val);
    }

    return 0;
}

void installScripts(){
    ScriptSystem* ss = Game::getScriptSystem();
    ss->addFunction("editor_updateFilter",updateFilter);
    ss->addFunction("editor_updateAllFilter",updateAllFilter);

}


#endif