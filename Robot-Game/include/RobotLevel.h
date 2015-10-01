#ifndef __HEADER_ROBOTLEVEL_
#define __HEADER_ROBOTLEVEL_

#include <LevelObjects/LevelObject.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/GameState/Level.h>
#include <ChibiEngine/Render/Particles/ParticleEffect.h>
#include <string>
#include <Character.h>
#include <CharacterReference.h>
#include <ChibiEngine/Common/SortedList.h>
#include <SFML/Audio.hpp>
#include <ChibiEngine/Physics/Physics.h>
#include <unordered_map>
#include <ChibiEngine/Input/Input.h>

class RobotCameraConstraints;
namespace game {

    class Interactive;
    class Layer;
    class RobotLevel;
    class CameraController;

    class LevelContactListener : public b2ContactListener {
    public:
        virtual ~LevelContactListener() {};
        virtual void BeginContact(b2Contact *contact);
        virtual void EndContact(b2Contact *contact);
        virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
    };

    class ProtagonistMove : public InputEvent {
    public:
        ProtagonistMove(RobotLevel *parent);
        virtual ~ProtagonistMove();
        virtual void execute(const std::string &eventName, State state);
        void setBlockInput(bool isBlocked);
    private:
        RobotLevel *parent;
        bool isBlocked;
    };


    class LightSource;
    class Logger;
    class CharacterReference;
    class RobotLevel : public Level {
    public:

        static RobotLevel *instance();
        static CameraController* camera();
        static CharacterReference* chars();


        RobotLevel(const std::string &worldFile);
        ~RobotLevel();

        inline ProtagonistMove* getInputListener();
        inline Character *getCharacter();
        inline Layer* getMainLayer();

        LevelObject *findObject(const std::string &name);
        LevelObject *findObject(int uid);
        void deleteObject(const std::string &name);
        RobotConfigType *getSettings();

        void draw(CameraCHandle camera) override;
        void toggleDebug();
        void togglePres();

        inline const Logger& getLogger() const;

        void viewInteractive(Interactive* current);
        void interact();
        void setDayTime(float v);

        inline int generateUid();
        template<class T> T* add2Index(T* obj);
    private:

        void load(const std::string &worldFile);

        Interactive* currentInteractive;
        const Logger logger;
        ProtagonistMove moveListener;
        bool drawDebug,drawPres;

        Color ambientLight, dayLight, nightLight;

        std::vector<Layer *> layers;
        Layer *mainLayer;
        Character *character;

        LevelContactListener contactListener;
        CharacterReference* charReference;
        CameraController* cameraController;

        // uid index
        int lastUid;
        //todo чистить его при удаление объекта
        std::unordered_map<int, LevelObject*> globalIndex;
    };

    inline ProtagonistMove* RobotLevel::getInputListener(){
        return &moveListener;
    }

    inline Character *RobotLevel::getCharacter() {
        return character;
    }

    inline const Logger& RobotLevel::getLogger() const{
        return logger;
    }

    inline int RobotLevel::generateUid(){
        lastUid++;
        return lastUid;
    }

    template<class T> T* RobotLevel::add2Index(T* obj){
        if(obj->getUid()>0)
            globalIndex[obj->getUid()]=obj;
        return obj;
    }

    inline Layer* RobotLevel::getMainLayer(){
        return mainLayer;
    }

}

#endif
