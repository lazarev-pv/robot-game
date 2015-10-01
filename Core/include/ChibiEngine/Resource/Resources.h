#ifndef __RESOURCES_HEADER_
#define __RESOURCES_HEADER_

#include <unordered_map>
#include <string>
#include <list>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <unordered_set>
#include <memory>

#include <ChibiEngine/Game.h>
#include <ChibiEngine/Resource/Texture.h>
#include <ChibiEngine/Resource/Font.h>
#include <ChibiEngine/Handles.h>

class ResourceDescriptionList;
class SpriteDescrType;
class FrameType;
class AnimationType;
class ResourcesListType;
class SpriteLinkType;
namespace game{

    class AnimatedSprite;
    class TexturedObject;
    class PatternSprite;
    class Resources;
    struct SpriteInfo {
        using CHandle = const SpriteInfo*;
        struct Animation;
        using AnimationCHandle = const Animation*;

        struct Frame;
        using FrameCHandle = const Frame*;

        struct Frame{

            static std::unique_ptr<const SpriteInfo::Frame> create4Animation(const FrameType* data, const Animation* parent);
            static std::unique_ptr<const SpriteInfo::Frame> createStandAlone(const FrameType* data, const Texture* t);
            static std::unique_ptr<const SpriteInfo::Frame> createDefault();

            AnimationCHandle parent;
            glm::vec2 topLeftPnt, bottomRightPnt;
        private:
            Frame(const FrameType* f, AnimationCHandle parent, TextureCHandle t);
            Frame();
        };

        struct Animation{

            Animation(const AnimationType* data, SpriteInfo::CHandle parent, int initialOffset);
            int initialOffset;
            bool cycled;
            int frameTime;
            std::string name;
            SpriteInfo::CHandle parent;
            std::vector<std::unique_ptr<const Frame>> frames;

            inline int getNumber() const {return initialOffset/20;};
        };

        SpriteInfo(const SpriteDescrType *data, ClockCHandle clock, TextureCHandle texture);
        SpriteInfo(const SpriteLinkType *data, ClockCHandle clock, TextureCHandle texture);
        SpriteInfo(const std::string& name, ClockCHandle clock, TextureCHandle texture);
        float top, bot,left,right,hw,hh;
        std::string initialAnimation;
        TextureCHandle texture;
        std::string name;
        std::unique_ptr<const Frame> frame;
        ClockCHandle clock; // for animated
        std::unordered_map<std::string, std::unique_ptr<const SpriteInfo::Animation>> animations;
    };

    class Resources : private UniqueClass {
    public:
        Resources(ResourcesListType* additionalResources);

        // finders for lazy-created singletons
        FontCHandle findFont(const std::string& name);
        TextureCHandle findTexture(const std::string& name);
        TexturedObjectHandle findSprite(const std::string& name);
        AnimatedSpriteHandle findAnimatedSprite(const std::string& name);

        //finder for available sprites
        SpriteInfo::CHandle findSpriteInfo(const std::string& name) const;
        std::string findResourcePath(const std::string& name);
        std::vector<std::string> findAtlasSprites(const std::string& name);

        // creators
        std::unique_ptr<PatternSprite> createPatternSprite(const std::string& name, float xRepeats, float yRepeats) const;

        // loaders - load list of resources
        void load(ResourceDescriptionList* list, const std::string& sourceFileName, bool useUIClock=false);
        void loadSprite(const std::string& file);

        inline const Logger& getLogger() const;
	private:

        TextureCHandle loadTexture(const std::string& filePath, const std::string& name="");
        void loadSprite(const std::string& file, SpriteLinkType* iter, bool useUIClock);

        Logger logger;

        // available resource's info (usual it's path)
        std::unordered_map<std::string, std::string> texturesInfo;
        std::unordered_map<std::string, std::string> fontsInfo;
        std::unordered_map<std::string, std::unique_ptr<const SpriteInfo>> spritesInfo;
        std::unordered_map<std::string, std::string> othersInfo;

        // created resources
        std::unordered_map<std::string, std::unique_ptr<const Font>> createdFonts;
        std::unordered_map<std::string, std::unique_ptr<const Texture>> createdTextures;
        std::unordered_map<std::string, std::unique_ptr<TexturedObject>> createdSprites;

        // additional indices
        std::unordered_map<std::string, AnimatedSpriteHandle> createdAnimatedSprites;
        std::unordered_set<std::string> loadedFiles;
        std::unordered_map<std::string, std::vector<std::string>> atlasSprites;
    };

    inline const Logger& Resources::getLogger() const{
        return logger;
    }

}

#endif
