#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <sstream>
#include <ChibiEngine/XMLParser/SettingsParser.h>
#include <ChibiEngine/Common/ParserUtils.h>

#if defined(MY_LINUX)
#include <dirent.h>
#endif

using namespace rapidxml;
using namespace game;
using namespace std;

const static string LOGGER_NAME = "FileSystem";
std::string FileSystem::pathPrefix = "";

FileSystem::FileSystem(std::string configFile):
        logger(Game::getLoggingSystem()->createLogger(LOGGER_NAME)){

    logger.trace("Load Settings data");
	string fileName(getPlatformRootPath(configFile));

    {
    	xml_document<> doc;
        custom_unique_ptr<char> buffer = readTextFile(fileName);
	    doc.parse<0>(buffer.get());
	    SettingsParserRoot root(&doc);
        dataRootName = root.settings->gameStates->dataRootDir.size()>0?
                    root.settings->gameStates->dataRootDir:"";
        dataRootName = pathPrefix+dataRootName;
    }

    logger.info("Resource System initialized");
}

std::string FileSystem::getPath(const std::string& fullName){
    string::size_type foundSlash = fullName.find_last_of("/\\");
    return (foundSlash!=string::npos)?fullName.substr(0,foundSlash):".";
}

std::string FileSystem::getExtension(const std::string& fullName){
    string::size_type  foundDot = fullName.find_last_of(".");
    return (foundDot!=string::npos)?fullName.substr(foundDot +1):"";
}

std::string FileSystem::getPureName(const std::string& fullName){
    string::size_type  foundDot = fullName.find_last_of(".");
    string::size_type  foundSlash = fullName.find_last_of("/\\");

    return  (foundDot && foundSlash)?fullName.substr(foundSlash+1,foundDot-foundSlash-1):
            (foundDot && !foundSlash)?fullName.substr(0,foundDot):
            (!foundDot && foundSlash)?fullName.substr(foundSlash):
                                      fullName;
}

void FileSystem::getFileList(std::vector<std::string>& result, std::string dirName, std::unordered_set<std::string>* extFilter){
#if defined(MY_LINUX)
    Game::getFileSystem()->logger.trace("getFileList for " + dirName);
    DIR* d = opendir(dirName.c_str());
    if(d){
        struct dirent* ent;
        while((ent = readdir(d))!=nullptr){
            string name(ent->d_name);
            if(name=="." || name=="..")
                continue;

            if(ent->d_type == DT_DIR){
                getFileList(result, dirName+"/"+name,extFilter);
            }

            if(extFilter!=nullptr){
                string ext = getExtension(name);
                auto it = extFilter->find(ext);
                if(it==extFilter->end()){
                    continue;
                }
            }

            result.push_back((dirName+"/"+name)/*.substr(2)*/);
        }
        closedir(d);
    }
#else
    Game::getFileSystem()->getLogger().error("getFileList not implemented");
#endif
}

ifstream& FileSystem::openFileStream(ifstream& file, const std::string& name,std::ios_base::openmode openMode){
    logger.debug("Opening file: %s", name.c_str());
	file.open(getFullName(name), openMode);
    return file;
}

custom_unique_ptr<char> FileSystem::readTextFile(const std::string& nm, const std::string& relatedPath){
	if(nm.size()<=0)
		return nullptr;

	std::string fullName = getFullName(relatedPath+"/"+nm);
    logger.debug("Reading file %s content", fullName.c_str());

    custom_unique_ptr<FILE> f (fopen(fullName.c_str(), "r"),[](FILE* p){fclose(p);});
	if(!f){
        logger.error("Can't read file %s", fullName.c_str());
		return nullptr;
	}

	fseek(f.get(), 0, SEEK_END);
	auto fsize = ftell(f.get());
	fseek(f.get(), 0, SEEK_SET);

    custom_unique_ptr<char> buffer ((char*)malloc(fsize + 1),[](char* p){free(p);});
	fread(buffer.get(), fsize, 1, f.get());
    {
        char* tmp = buffer.get();
        tmp[fsize] = 0;
    }
    logger.trace("File have been read");
	return buffer;
}

std::string FileSystem::getFullName(const std::string& name){
	return dataRootName+name;
}
std::string FileSystem::getFullName(const char* name){
	return dataRootName+name;
}

std::string FileSystem::getPlatformRootPath(const std::string& name){
	return pathPrefix+name;
}
