#ifndef __CAMERA_H_
#define __CAMERA_H_

#include <glm/glm.hpp>
#include <ChibiEngine/Common/EventSupportable.h>
#include <vector>
#include <ChibiEngine/Render/RenderHandles.h>
#include <ChibiEngine/Common/ClassUtils.h>

class CameraType;
namespace game {

    class Camera :  private EventSupportableImpl, private UniqueClass {
    public:

        Camera(CameraType *settings);

        void moveCamera(glm::vec3 vec); /** fire position events */
        void setPosition(const glm::vec2 &pos);
        void setPosition(const glm::vec3 &position);

        const glm::mat4 &getMVPMatrix() const;
        glm::vec3 convertToWorldCoordinates(glm::vec2 screenCoordinates) const;
        glm::vec2 calcScale(float hw, float hh);
        inline const glm::mat4 &getProjectionMatrix() const;
        inline const glm::mat4 &getViewMatrix() const;
        inline float getNear() const;
        inline float getFar() const;
        glm::vec3 getPosition() const;

    private:
        virtual void updateMatrix();

        glm::vec3 up;
        glm::vec3 position;
        float near, far;
        float fov;
        glm::mat4 viewMatrix;
        glm::mat4 projmatrix;
        glm::mat4 mvp;
        float screenZ;
    };

    inline const glm::mat4 &Camera::getProjectionMatrix() const{
        return projmatrix;
    }

    inline const glm::mat4 &Camera::getViewMatrix() const{
        return viewMatrix;
    }

    inline float Camera::getNear() const{
        return near;
    }

    inline float Camera::getFar() const{
        return far;
    }

}

#endif
