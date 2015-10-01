#include <ChibiEngine/Resource/LevelResources.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/XMLParser/ResourcesDescriptionParser.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Log/LoggingSystem.h>

using namespace game;
using namespace std;

LevelResources::LevelResources()
:logger(Game::getLoggingSystem()->createLogger("LevelResources")){

}

LevelResources::Links::Links(LinkType* data)
    :name(data->name),file(data->file){
}

LevelResources::Links::Links(const std::string& name, const std::string& file)
    :name(name),file(file){
}

LevelResources::Sprites::Sprites(SpriteLinkType* data)
        :Links(data->name,data->file),
         w(data->w),
         h(data->h),
         top(data->left),
         left(data->left),
         right(data->right),
         bottom(data->bottom){
}

LevelResources::Sprites::Sprites(const std::string& fileName)
        :Links(FileSystem::getPureName(fileName),fileName),
         w(-1),
         h(-1),
         top(-1),
         left(-1),
         right(-1),
         bottom(-1){
}


LinkType* LevelResources::Links::saveAsResource() const{
    LinkType* l = new LinkType();
    l->file = file;
    l->name = name;
    return l;
}

SpriteLinkType* LevelResources::Sprites::saveAsSrite() const{
    SpriteLinkType* l = new SpriteLinkType();
    l->file = file;
    l->name = name;
    l->w = w;
    l->h = h;
    l->top = top;
    l->left = left;
    l->right = right;
    l->bottom = bottom;
    return l;
}

void LevelResources::loadAdditionalSprites(const std::vector<std::string>& fileNames, const std::string& /*relativePath*/){
    logger.error("loadAdditionalSprites - NOT IMPLEMENTED");
    Resources* r = Game::getResources();
    string prefix = "";//relativePath+"///";
    for(auto name : fileNames){
        auto iter = sprites.find(prefix+name);
        logger.trace("Processing file " + prefix + name);
        if(iter==sprites.end()){
            sprites.emplace(prefix+name,name);
            r->loadSprite(prefix+name);
        }else{
            logger.trace("sprite already loaded " + name);
        }
    }
}

void LevelResources::loadResources(ResourceDescriptionList* list, const std::string& sourceFile){
    if(list == nullptr){
        return;
    }

    string prefix=FileSystem::getPath(sourceFile)+"/";
    Game::getResources()->load(list, sourceFile);
    for(auto iter : list->font){
        fonts.emplace(prefix+iter->file,iter);
    }
    for(auto iter : list->texture){
        texture.emplace(prefix+iter->file,iter);
    }
    for(auto iter : list->resource){
        resource.emplace(prefix+iter->file,iter);
    }
    for(auto iter : list->sprite){
        sprites.emplace(prefix+iter->file,iter);
    }
    this->sourceFile = sourceFile;

    /* vector<std::string> fileNames;
     string relativePath = FileSystem::getPath(sourceFile);
     unordered_set<string> exts({"atlas","png"});
     FileSystem::getFileList(fileNames, relativePath, &exts);
     loadAdditionalSprites(fileNames,relativePath);*/
}

void LevelResources::getSpriteNames(std::vector<std::string>& names) const{
    for(auto& iter : sprites){
        if(iter.second.name.size()>0) {
            names.push_back(iter.second.name);
        }else if(FileSystem::getExtension(iter.second.file)=="atlas"){
            vector<string> ls = Game::getResources()->
                    findAtlasSprites(FileSystem::getPath(sourceFile)+"/"+iter.second.file);
            for(auto& nm : ls){
                names.push_back(nm);
            }
        }else { //nor atlas, nor has name
            names.push_back(FileSystem::getPureName(iter.second.file));
        }
    }
}

void LevelResources::saveResources(ResourceDescriptionList *destination){
    for(const auto& iter : fonts){
        destination->font.push_back(iter.second.saveAsResource());
    }
    for(const auto& iter : texture){
        destination->texture.push_back(iter.second.saveAsResource());
    }
    for(const auto& iter : resource){
        destination->resource.push_back(iter.second.saveAsResource());
    }
    for(const auto& iter : sprites){
        destination->sprite.push_back(iter.second.saveAsSrite());
    }
}