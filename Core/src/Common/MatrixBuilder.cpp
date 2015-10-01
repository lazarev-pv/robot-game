#include <ChibiEngine/Common/MatrixBuilder.h>
#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

using namespace game;
using namespace glm;
using namespace std;

mat4 MatrixBuilder::toMatrix(){
    auto res = _camera?_camera->getMVPMatrix():mat4(1.0f);
    res*=glm::translate(_pos);
    for(auto rotation = rotations.crbegin();rotation != rotations.crend();rotation++){
        res*=glm::translate(rotation->pivotPnt);
        res*=glm::rotate(rotation->angle,glm::vec3(0.0f,0.0f,1.0f));
        res*=glm::translate(-rotation->pivotPnt);
     }
    res*=glm::scale(vec3(_reflected?-_scale.x:_scale.x,_scale.y,1.0f));
    return res;
}

glm::mat4 game::createMatrix(const glm::vec3& pos, const glm::vec2& scale,CameraCHandle camera){
    return (camera?camera->getMVPMatrix():mat4(1.0f))
            * glm::translate(pos)
            * glm::scale(vec3(scale.x,scale.y,1.0f));
}

glm::mat4 game::createMatrix(const glm::vec3& pos, float angle, const glm::vec2& scale,CameraCHandle camera){
    return (camera?camera->getMVPMatrix():mat4(1.0f))
            * glm::translate(pos)
            * glm::rotate(angle,glm::vec3(0.0f,0.0f,1.0f))
            * glm::scale(vec3(scale.x,scale.y,1.0f));
}