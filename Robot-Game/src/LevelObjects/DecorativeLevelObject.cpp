#include <LevelObjects/DecorativeLevelObject.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Render/Sprites/PatternSprite.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <LevelObjects/Box.h>
#include <cmath>


#define GLM_FORCE_RADIANS
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtx/transform.hpp>
#include <ChibiEngine/Common/MatrixBuilder.h>

using namespace std;
using namespace game;
using namespace glm;
using namespace rapidxml;

const static vec3 NULL_VEC(0,0,0);

DecorativeLevelObject::DecorativeLevelObject(int uid, ArtType* data, float z,ObjectFixture* parent, Layer* layer)
:LevelObject(uid,layer,data->zOrder,data->name),
 offset(parse(data->pos)),
 scale(parse(data->scale)),
 rotation(data->rotation),
 parent(parent),
 color(game::parse(data->color)){
	if(data->repeat!=nullptr){
		presentation = Game::getResources()->createPatternSprite(data->image->name,data->repeat->x,data->repeat->y);
	}else{
		presentation = Game::getResources()->createPatternSprite(data->image->name,1.0,1.0);
	}
}

void DecorativeLevelObject::draw(CameraCHandle camera,const Color& ambientLight){
    if(!isEnabled()) {
        return;
    }
    if(parent!=nullptr) {
        //Добавить ещё транслейт относительной самой фикстуры

        glm::mat4 result = camera->getMVPMatrix()
                * glm::translate(parent->getFullOffset())
                * glm::translate(-parent->getFixtureOffset())
                * glm::rotate(mat4(1.0),parent->getAngle(),glm::vec3(0.0f,0.0f,1.0f))
                * glm::translate(parent->getFixtureOffset()+offset)
                * glm::scale(vec3(scale.x,scale.y,1.0f))
        ;

        presentation->draw(result, ambientLight*color);
    }else{
        presentation->draw(createMatrix(offset, rotation, scale, camera), ambientLight*color);
    }
}

glm::vec3 DecorativeLevelObject::getPosition(){
    return (parent!= nullptr)?
            parent->getFixtureOffset()+offset:
            offset;
}