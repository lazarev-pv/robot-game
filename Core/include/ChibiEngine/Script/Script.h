#ifndef __H_SCRIPT_SYSTEM_
#define __H_SCRIPT_SYSTEM_

#include <string>
#include <functional>
#include <unordered_set>
#include <vector>
#include <ChibiEngine/Log/Logger.h>
#include <memory>
#include <glm/glm.hpp>
#include <ChibiEngine/Common/ClassUtils.h>
#include <ChibiEngine/Render/Color.h>

class ScriptsEventListener;
struct lua_State;
namespace game{

    class ScriptSystem : private UniqueClass {
    public:

        ScriptSystem();
        ~ScriptSystem();

        void addFunction(const std::string& name, int(* func)(lua_State *));
        void loadFile(const std::string& fileName);
        void doScript(const std::string& functionName);
        void doScript(const std::string& funcName, const std::vector<std::function<void(lua_State*)>> args);
        void doString(const std::string& expression);

        inline const Logger& getLogger()const;
        inline ScriptsEventListener *getListener();
    private:
        lua_State* lua;
        Logger logger;
        std::unordered_set<std::string> loadedFiles;
        std::unique_ptr<ScriptsEventListener> listener;

    };

    inline const Logger& ScriptSystem::getLogger()const{
        return logger;
    }

    inline ScriptsEventListener* ScriptSystem::getListener(){
        return listener.get();
    }



    const std::function<void(lua_State*)> arg(const std::string& val);
    const std::function<void(lua_State*)> arg(int val);

    Color getColor(lua_State* lua, int parmIndex);
    float getFloat(lua_State* lua, int parmIndex, const char* name);

}
#endif