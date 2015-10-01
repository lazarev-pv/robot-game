#ifndef __RESOURCES_SYSTEM_H
#define    __RESOURCES_SYSTEM_H

#include <map>
#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <ChibiEngine/Log/Logger.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Common/RAIIUtils.h>

class SettingsParserRoot;
namespace game {

    class FileSystem : private UniqueClass {
    public:

        static std::string getPlatformRootPath(const std::string& name);
        static std::string getPath(const std::string& fullName);
        static std::string getExtension(const std::string& fullName);
        static std::string getPureName(const std::string& fullName);
        static void getFileList(std::vector<std::string>& result, std::string dirName=".", std::unordered_set<std::string>* extFilter=nullptr);

        FileSystem(std::string configFile);

        std::string getFullName(const std::string &name);
        std::string getFullName(const char *name);
        inline const Logger& getLogger();
        std::ifstream& openFileStream(std::ifstream &file, const std::string& name, std::ios_base::openmode openMode);
        custom_unique_ptr<char> readTextFile(const std::string &name, const std::string& relatedPath=".");

    private:
        friend class Game;

        static std::string pathPrefix;
        Logger logger;
        std::string dataRootName;
    };


    inline const Logger& FileSystem::getLogger(){
        return logger;
    }
}
#endif

