#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Clock/EventSystem.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/XMLParser/AtlasParser.h>
#include <ChibiEngine/Render/Sprites/NineSliceSprite.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/Render/Sprites/AnimatedSprite.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/Render/Sprites/PatternSprite.h>
#include <ChibiEngine/XMLParser/ResourcesDescriptionParser.h>

using namespace game;
using namespace std;
using namespace glm;
using namespace rapidxml;


SpriteInfo::Frame::Frame(const FrameType* f,const Animation* parent, const Texture* t):
    parent(parent){
    int w = t->getWidth()-1;
    int h = t->getHeight()-1;

    topLeftPnt=vec2((float)f->x1/w, 1-(float)f->y1/h);
    bottomRightPnt = vec2((float)f->x2/w, 1-(float)f->y2/h);
}

std::unique_ptr<const SpriteInfo::Frame> SpriteInfo::Frame::createDefault(){
    return unique_ptr<const SpriteInfo::Frame>(new Frame());
}

SpriteInfo::Frame::Frame():
    parent(nullptr),topLeftPnt(0,1),bottomRightPnt(1,0){
}

std::unique_ptr<const SpriteInfo::Frame> SpriteInfo::Frame::create4Animation(const FrameType* data, const Animation* parent){
   return unique_ptr<const SpriteInfo::Frame>(new Frame(data,parent,parent->parent->texture));
}

std::unique_ptr<const SpriteInfo::Frame> SpriteInfo::Frame::createStandAlone(const FrameType* data, const Texture* t){
    return unique_ptr<const SpriteInfo::Frame>(new Frame(data,nullptr,t));
}

SpriteInfo::Animation::Animation(const AnimationType* data, SpriteInfo::CHandle parent,int initialOffset)
    :initialOffset(initialOffset),
    cycled(data->cycled),
    frameTime(data->frameTime),
    name(data->name),
    parent(parent){
    for(auto f : data->frame){
        frames.push_back(Frame::create4Animation(f,this));
    }
}

SpriteInfo::SpriteInfo(const SpriteDescrType *data, ClockCHandle clock, TextureCHandle texture):
    top(data->top),
    bot(data->bottom),
    left(data->left),
    right(data->right),
    hw(data->w*0.5f),
    hh(data->h*0.5f),
    initialAnimation(data->initialAnimation),
    texture(texture),
    name(data->name),
    frame(data->frame!=nullptr?Frame::createStandAlone(data->frame, texture):
            data->animation.size()==0?Frame::createDefault():
            nullptr),
    clock(clock){

    if(hw<0){
        hw = texture->getWidth()*0.5f;
    }
    if(hh<0){
        hh = texture->getHeight()*0.5f;
    }

    int offset = 0;
    for(auto a : data->animation){
        animations.emplace(a->name,make_unique<const SpriteInfo::Animation>(a,this,offset));
        offset += (5 * 4 * a->frame.size());
    }

}

SpriteInfo::SpriteInfo(const std::string& name, ClockCHandle clock, TextureCHandle texture):
        top(-1),
        bot(-1),
        left(-1),
        right(-1),
        hw(texture->getWidth()*0.5f),
        hh(texture->getHeight()*0.5f),
        initialAnimation(),
        texture(texture),
        name(name),
        frame(Frame::createDefault()),
        clock(clock){
}

SpriteInfo::SpriteInfo(const SpriteLinkType *data, ClockCHandle clock, TextureCHandle texture):
        top(data->top),
        bot(data->bottom),
        left(data->left),
        right(data->right),
        hw(data->w*0.5f),
        hh(data->h*0.5f),
        initialAnimation(),
        texture(texture),
        name(data->name),
        frame(Frame::createDefault()),
        clock(clock){

    if(name.size()==0){
        name=FileSystem::getPureName(data->file);
    }
    if(hw<0){
        hw = texture->getWidth()*0.5f;
    }
    if(hh<0){
        hh = texture->getHeight()*0.5f;
    }
}


FontCHandle Resources::findFont(const std::string& name){
    auto existed = createdFonts.find(name);
    if(existed!=createdFonts.end()){
        return existed->second.get();
    }

    auto info = fontsInfo.find(name);
    if(info!=fontsInfo.end()){
        const Font* font = new Font(info->second.c_str());
        logger.debug("Created Font " + name);
        createdFonts.emplace(name, unique_ptr<const Font>(font));
        return font;
    };

    logger.error("Font %s not found!", name.c_str());
    return nullptr;
}

TextureCHandle Resources::loadTexture(const std::string& filePath, const std::string& _name){
    string name = _name.size()>0?_name:FileSystem::getPureName(filePath);
    texturesInfo[name]=filePath;
    const Texture* tex = new Texture(filePath, _name);
    logger.debug("Created Texture " + name);
    createdTextures.emplace(name, unique_ptr<const Texture>(tex));
    return tex;
}

TextureCHandle Resources::findTexture(const std::string& name){
    auto existed = createdTextures.find(name);
    if(existed!=createdTextures.end()){
        return existed->second.get();
    }

    auto info = texturesInfo.find(name);
    if(info!=texturesInfo.end()){
        const Texture* texture = new Texture(info->second, name);
        logger.debug("Created Texture " + name);
        createdTextures.emplace(name, unique_ptr<const Texture>(texture));
        return texture;
    }

    logger.error("Texture %s not found!", name.c_str());
    return nullptr;
}

TexturedObjectHandle Resources::findSprite(const std::string& name){
    auto existed = createdSprites.find(name);
    if(existed!=createdSprites.end()){
        return existed->second.get();
    }

    SpriteInfo::CHandle info = findSpriteInfo(name);
    if(info==nullptr)
        return nullptr;

    if(info->animations.size()==0){
        TexturedObject* obj = new TexturedObject(info);
        logger.debug("Created Sprite " + name);
        createdSprites.emplace(name, unique_ptr<TexturedObject>(obj));
        return obj;
    }else{
        AnimatedSprite* obj = new AnimatedSprite(info);
        logger.debug("Created AnimatedSprite " + name);
        createdSprites.emplace(name, unique_ptr<TexturedObject>(obj));
        createdAnimatedSprites.emplace(name,obj);
        return obj;
    }
}

AnimatedSpriteHandle Resources::findAnimatedSprite(const std::string& name){
    auto existed = createdAnimatedSprites.find(name);
    if(existed!=createdAnimatedSprites.end()){
        return existed->second;
    }

    SpriteInfo::CHandle info = findSpriteInfo(name);
    if(info==nullptr)
        return nullptr;

    if(info->animations.size()==0){
        logger.error("SpriteInfo %s  contains no animation!", name.c_str());
        return nullptr;
    }

    AnimatedSpriteHandle obj = new AnimatedSprite(info);
    logger.debug("Created AnimatedSprite " + name);
    createdSprites.emplace(name,unique_ptr<TexturedObject>(obj));
    createdAnimatedSprites[name]=obj;
    return obj;
}

SpriteInfo::CHandle Resources::findSpriteInfo(const std::string& name) const{
    const auto infoI = spritesInfo.find(name);
    if(infoI==spritesInfo.end()){
        logger.error("SpriteInfo %s not found!", name.c_str());
        return nullptr;
    }
    return infoI->second.get();
}

std::string Resources::findResourcePath(const std::string& name){
    return othersInfo[name];
}

std::vector<std::string> Resources::findAtlasSprites(const std::string& name){
    return atlasSprites[name];
}

std::unique_ptr<PatternSprite> Resources::createPatternSprite(const std::string& name, float xRepeats, float yRepeats) const {
    SpriteInfo::CHandle data = findSpriteInfo(name);
    logger.debug("Created PatternSprite %s repeats %f %f ", name.c_str(), xRepeats, yRepeats);
    return make_unique<PatternSprite>(data, xRepeats, yRepeats);
}

void Resources::loadSprite(const std::string& file){
    loadSprite(file, nullptr,false);
}

void Resources::loadSprite(const std::string& file, SpriteLinkType* iter, bool useUIClock){
    logger.debug("Loading Sprite from file " + file);

    string ext = FileSystem::getExtension(file);
    ClockCHandle clock = useUIClock? Game::getUserInterface()->getClock():Game::getLevelClock();
    if(ext=="atlas"){
        logger.trace("found atlas " + file);
        auto root(createRoot<AtlasParserRoot>(file));
        vector<string> spritesInAtlas;
        TextureCHandle t = loadTexture(FileSystem::getPath(file)+"/"+root.texture->file);
        for(auto iter : root.texture->sprite) {
            SpriteInfo::CHandle info = new SpriteInfo(iter, clock, t);
            logger.trace("Created atlas sprite " + info->name);
            spritesInfo.emplace(info->name,unique_ptr<const SpriteInfo>(info));
            spritesInAtlas.push_back(info->name);
        }
        atlasSprites[file]=spritesInAtlas;
    }else if(ext=="png"){
        SpriteInfo::CHandle info = nullptr;

        if(iter!=nullptr){ // ресурс был заранее описан в дескрипторе
            TextureCHandle tex = loadTexture(file,iter->name);
            info = new SpriteInfo(iter, clock, tex);
        }else{ // ресурс найден в файловой системе
            assert(false); //"not implemented case"
            TextureCHandle tex = loadTexture(file,iter->name);
            info = new SpriteInfo(FileSystem::getPureName(file), clock, tex);
        }
        logger.trace("Created standalone sprite " + info->name);
        spritesInfo.emplace(info->name,unique_ptr<const SpriteInfo>(info));
    }else{
        logger.warn("Extension %s unsupporting - passing", ext.c_str());
    }
}

void Resources::load(ResourceDescriptionList* list, const std::string& sourceFileName,bool useUIClock){
    logger.debug("Loading resources from file " + sourceFileName);

    if(loadedFiles.find(sourceFileName)!=loadedFiles.end()) {
        logger.warn("File %s already loaded ", sourceFileName.c_str());
        return;
    }
    string prefix=FileSystem::getPath(sourceFileName)+"/";
    for(LinkType* iter : list->font){
        fontsInfo[iter->name]=prefix+iter->file;
    }
    for(LinkType* iter : list->texture){
        texturesInfo[iter->name]=prefix+iter->file;
    }
    for(LinkType* iter : list->resource){
        othersInfo[iter->name]=prefix+iter->file;
    }
    for(SpriteLinkType* iter : list->sprite){
        loadSprite(prefix+iter->file, iter, useUIClock);
    }
}

Resources::Resources(ResourcesListType* additionalResources)
:logger(Game::getLoggingSystem()->createLogger("Resources")){
    if(additionalResources!= nullptr) {
        for (auto iter : additionalResources->description){
            string fullFileName = Game::getConfigPath()+"/"+iter->file;
            auto root(createRoot<ResourcesDescriptionParserRoot>(fullFileName));
            load(root.resources,fullFileName,false); //todo не очень хорошо, хотя все равно два раза этот файл грузится не будет
        }
    }
}

