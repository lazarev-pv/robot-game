
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/Resource/FileSystem.h>

using namespace game;
using namespace std;

const static string SETTINGS_FILE_NAME = "log.config";
const static string none = "none";
const static string error = "error";
const static string warn = "warn";
const static string info = "info";
const static string debug = "debug";
const static string trace = "trace";

LoggingSystem::LoggingSystem(){
    logLevelStringPresentations[none] = Logger::NONE;
    logLevelStringPresentations[error] = Logger::ERROR;
    logLevelStringPresentations[warn] = Logger::WARN;
    logLevelStringPresentations[info] = Logger::INFO;
    logLevelStringPresentations[debug] = Logger::DEBUG;
    logLevelStringPresentations[trace] = Logger::TRACE;
    loggersLevel["Default"] = Logger::INFO;

    ifstream file(FileSystem::getPlatformRootPath("log.config"),ifstream::in);
    while(!file.eof()){
        string name,level;
        file>>name>>level;

        map<string,Logger::LogLevel>::iterator iter = logLevelStringPresentations.find(level);
        if(iter == logLevelStringPresentations.end())
            loggersLevel[name]=Logger::NOT_SPECIFIED;
        else
            loggersLevel[name] = iter->second;
    }
}

Logger LoggingSystem::createLogger(const std::string &name){
    map<string,Logger::LogLevel>::iterator iter = loggersLevel.find(name);
    Logger::LogLevel level = (iter!= loggersLevel.end())?iter->second:loggersLevel["Default"];
    return Logger(level,name);
}
