#define GLM_FORCE_RADIANS

#include <glm/ext.hpp>
#include <ChibiEngine/Game.h>

#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Render/ScreenSystem.h>

using namespace game;
using namespace glm;
using namespace std;

Camera::Camera(CameraType *camera) :
        up(vec3(0.0f, 1.0f, 0.0f)),
        position(camera->x, camera->y, camera->z),
        near(camera->near),
        far(camera->far),
        fov(camera->fov) {

    updateMatrix();
}

void Camera::updateMatrix() {
    projmatrix = glm::perspective(
            fov,
            static_cast<float>(Game::getScreen()->getWidth()) / static_cast<float>(Game::getScreen()->getHeight()),
            near, far);

    viewMatrix = glm::lookAt(position, vec3(position.x, position.y, position.z - 10.0), up);
    mvp = projmatrix * viewMatrix;

    screenZ = glm::project(vec3(0, 0, 0), viewMatrix, projmatrix, vec4(-1, -1, 2, 2)).z;
}

const glm::mat4 &Camera::getMVPMatrix() const {
    return mvp;
}

void Camera::moveCamera(vec3 dir) {
    position += dir;
    updateMatrix();
    fireEvent(EventSupportable::MOVE_EVENT);
}

void Camera::setPosition(const vec3 &position) {
    this->position = position;
    updateMatrix();
}

void Camera::setPosition(const vec2 &pos) {
    this->position.x = pos.x;
    this->position.y = pos.y;
    updateMatrix();
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::convertToWorldCoordinates(glm::vec2 screenCoordinates) const{
    return glm::unProject(vec3(screenCoordinates, screenZ),
            viewMatrix,
            projmatrix,
            vec4(-1, -1, 2, 2));
}

glm::vec2 Camera::calcScale(float _hw, float _hh) {
    float hw = _hw * Game::getScreen()->getWidthDensity();
    float hh = _hh * (2.0 / Game::getScreen()->getHeight());

    vec3 tl = convertToWorldCoordinates(vec2(-hw, hh));
    vec3 br = convertToWorldCoordinates(vec2(hw, -hh));

    float worldHW = 0.5 * (br.x - tl.x);
    float worldHH = 0.5 * (tl.y - br.y);
    return vec2(worldHW / _hw, worldHH / _hh);
}
