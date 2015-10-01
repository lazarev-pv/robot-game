#ifndef __LEVEL_RESOURCES_HEADER_
#define __LEVEL_RESOURCES_HEADER_

#include <string>
#include <vector>
#include <unordered_map>
#include <ChibiEngine/Log/Logger.h>
#include <ChibiEngine/Common/ClassUtils.h>

class LinkType;
class SpriteLinkType;
class ResourceDescriptionList;
namespace game {

    class LevelResources : private UniqueClass {
    public:
        LevelResources();
        void loadResources(ResourceDescriptionList *list, const std::string& sourceFile);
        void loadAdditionalSprites(const std::vector<std::string>& fileNames, const std::string& relativePath);
        void saveResources(ResourceDescriptionList *destination);

        void getSpriteNames(std::vector<std::string>& names) const;
    private:

        struct Links {
            Links(LinkType *data);
            LinkType *saveAsResource() const;

            std::string name, file;
        protected:
            Links(const std::string& name, const std::string& file);
        };

        struct Sprites : Links {
            Sprites(SpriteLinkType *data);
            Sprites(const std::string& fileName);
            SpriteLinkType *saveAsSrite() const;

            float w, h, top, left, right, bottom;
        };

        Logger logger;
        std::unordered_map<std::string, Links> fonts, texture, resource;
        std::unordered_map<std::string, Sprites> sprites;
        std::string sourceFile;

    };
}

#endif