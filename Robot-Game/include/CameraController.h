#ifndef __CAMERA_CONTROLLER_H_
#define __CAMERA_CONTROLLER_H_

#include "LevelObjects/LevelObject.h"
#include <ChibiEngine/Clock/Clock.h>

class RobotCameraSettings;
class CameraConfigType;
namespace game{

    class CameraController{
    public:
        CameraController(RobotCameraSettings* globalSettings, CameraConfigType* levelSettings);
        ~CameraController();

        inline void setHook(LevelObject* hook, const glm::vec2& offset);
        inline void setVelocity(const glm::vec3 vel);

        inline void setEasingPow(int easingPow);
    private:
        int updateId;
        void update();

        float left, right, top, bottom;
        LevelObject* hook;
        glm::vec3 velocity;
        glm::vec3 offsetVelocity;
        glm::vec3 offset;
        int easingPow = 1;
    };

    inline void CameraController::setHook(LevelObject* hook, const glm::vec2& offset){
        this->hook = hook;
        this->offset = glm::vec3(offset,0);
    }

    inline void CameraController::setVelocity(const glm::vec3 vel){
        CameraController::velocity = vel;
    }

    inline void CameraController::setEasingPow(int easingPow){
        CameraController::easingPow = easingPow;
    }
}

#endif

