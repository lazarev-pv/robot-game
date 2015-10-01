#ifndef __ROBOT_LEVEL_H_
#define __ROBOT_LEVEL_H_

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <LevelObjects/Interactive.h>
#include <ChibiEngine/Render/Color.h>

class WorldLayers;
class b2Body;
namespace game {

    class RobotLevel;
    class ParticleEffect;
    class LightSource;
    class LevelObject;
    class Layer {
    public:
        Layer(WorldLayers *layer, RobotLevel* level);
        ~Layer();

        inline float getZ() const;
        inline const std::string& getName() const;
        void add(LightSource* obj);
        void add(LevelObject* obj);
        void add(Interactive* obj);
        void draw(const Color &ambientLight);
        void setDrawDebug(bool val);
        void setDrawPres(bool val);
        void resort();

        void deleteObject(LevelObject* lo);
        b2Body* findPhysBody(const std::string& name);
        LevelObject* findObject(const std::string& name);
    private:
        LevelObject* add2Index(LevelObject* obj);

        RobotLevel* level;
        float z;
        std::string name;
        std::vector<LevelObject *> drawList;
        std::vector<LevelObject *> deleteList;
        std::vector<ParticleEffect *> particles;
        std::vector<LightSource *> lights;
        std::unordered_map<std::string, LevelObject *> objectIndex;
        int frameId, lightFrameId;
    };


    float Layer::getZ() const{
        return z;
    }

    const std::string& Layer::getName() const{
        return name;
    }

}

#endif