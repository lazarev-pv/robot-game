#include <ChibiEngine/Render/Sprites/DrawableObject.h>

using namespace std;
using namespace glm;
using namespace game;


DrawableObject::DrawableObject(const glm::vec2& size, const std::string& name)
:size(size),name(name){
}

float DrawableObject::getHW() const{
	return size.x;
}

float DrawableObject::getHH() const{
	return size.y;
}

//Возвращает название изображения
const std::string& DrawableObject::getName() const{
    return name;
}

const ShaderValues& DrawableObject::getShaderValue() const{
	return sv;
}
