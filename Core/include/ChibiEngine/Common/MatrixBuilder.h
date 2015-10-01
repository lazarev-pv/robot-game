#ifndef __CHIBIENGINE_COMMON_MATRIX_UTILS_
#define __CHIBIENGINE_COMMON_MATRIX_UTILS_

#include <glm/glm.hpp>
#include <ChibiEngine/Render/Camera.h>
#include <vector>
#include "MathUtils.h"

namespace game{

    glm::mat4 createMatrix(const glm::vec3& pos, float angle, const glm::vec2& scale,CameraCHandle camera=nullptr);
    glm::mat4 createMatrix(const glm::vec3& pos, const glm::vec2& scale,CameraCHandle camera=nullptr);
    inline glm::mat4 createMatrix(const glm::vec2& pos, float angle, const glm::vec2& scale,CameraCHandle camera=nullptr);
    inline glm::mat4 createMatrix(const glm::vec2& pos, const glm::vec2& scale,CameraCHandle camera=nullptr);

    class MatrixBuilder {
    public:
        inline MatrixBuilder& pos(const glm::vec2& val);
        inline MatrixBuilder& pos(const glm::vec3& val);
        inline MatrixBuilder& pos(const glm::vec4& val);
        inline MatrixBuilder& scale(const glm::vec2& val);
        inline MatrixBuilder& scale(float hw, float hh);

        // Добвляет поворот
        inline MatrixBuilder& rotate(float angle);
        inline MatrixBuilder& rotate(float angle, const glm::vec2& val);
        inline MatrixBuilder& rotate(float angle, const glm::vec3& val);
        inline MatrixBuilder& rotate(float angle, const glm::vec4& val);

        inline MatrixBuilder& reflected(bool value);
        inline MatrixBuilder& camera(CameraCHandle camera);

        glm::mat4 toMatrix();
    private:
        struct Rotation{
            float angle;
            glm::vec3 pivotPnt;
        };
        std::vector<Rotation> rotations;

        glm::vec3 _pos=glm::vec3(0.0f,0.0f,0.0f);
        glm::vec2 _scale=glm::vec2(1.0f,1.0f);
        bool _reflected=false;
        CameraCHandle _camera = nullptr;
    };
}

inline game::MatrixBuilder& game::MatrixBuilder::scale(const glm::vec2& val){
    _scale=val;
    return *this;
}
inline game::MatrixBuilder& game::MatrixBuilder::scale(float hw, float hh){
    _scale=glm::vec2(hw,hh);
    return *this;
}

inline game::MatrixBuilder& game::MatrixBuilder::pos(const glm::vec2& val){
    _pos=glm::vec3(val,0);
    return *this;
}

inline game::MatrixBuilder& game::MatrixBuilder::pos(const glm::vec3& val){
    _pos=val;
    return *this;
}
inline game::MatrixBuilder& game::MatrixBuilder::pos(const glm::vec4& val){
    _pos = toVec3(val);
    return *this;
}

inline game::MatrixBuilder& game::MatrixBuilder::rotate(float angle){
    rotations.push_back(Rotation{angle,glm::vec3(0,0,0)});
    return *this;
}


inline game::MatrixBuilder& game::MatrixBuilder::rotate(float angle, const glm::vec2& val){
    rotations.push_back(Rotation{angle,glm::vec3(val,0)});
    return *this;
}

inline game::MatrixBuilder& game::MatrixBuilder::rotate(float angle, const glm::vec3& val){
    rotations.push_back(Rotation{angle,val});
    return *this;
}

inline game::MatrixBuilder& game::MatrixBuilder::rotate(float angle, const glm::vec4& val){
    rotations.push_back(Rotation{angle, toVec3(val)});
    return *this;
}

inline game::MatrixBuilder& game::MatrixBuilder::reflected(bool value){
    _reflected = value;
    return *this;
}

inline game::MatrixBuilder& game::MatrixBuilder::camera(CameraCHandle val){
    _camera = val;
    return *this;
}

inline glm::mat4 game::createMatrix(const glm::vec2& pos, float angle, const glm::vec2& scale,CameraCHandle camera){
    return createMatrix(glm::vec3(pos,0), angle, scale, camera);
}
inline glm::mat4 game::createMatrix(const glm::vec2& pos, const glm::vec2& scale,CameraCHandle camera){
    return createMatrix(glm::vec3(pos,0), scale, camera);
}

#endif