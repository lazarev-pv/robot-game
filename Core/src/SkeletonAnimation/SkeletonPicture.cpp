#include <ChibiEngine/SkeletonAnimation/SkeletonPicture.h>
#include <ChibiEngine/SkeletonAnimation/SkeletonJoint.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/transform.hpp>
#include <ChibiEngine/Common/ParserUtils.h>

using namespace game;
using namespace glm;
using namespace std;

SkeletonPicture::SkeletonPicture(SkeletonSlot* parent, BodyPartType* data, const std::string& name):
        SkeletonPicture(
                parent,
                name,
                Game::getResources()->findSprite(data->pres),
                vec3(parse(data->pos),0),
                parse(data->scale),
                data->rotation
            ){
}

SkeletonPicture::SkeletonPicture(SkeletonSlot* parent,
        const std::string& name,
        TexturedObjectCHandle pres,
        const glm::vec3& pos,
        const glm::vec2& size,
        float rot):
    parent(parent),
    pres(pres),
    name(name){

    resetInitMatrix(pos, size, rot);
    refreshDrawMatrix();

}

void SkeletonPicture::resetInitMatrix(const glm::vec3& pos, const glm::vec2& scale, float rot){
    initMatrix= glm::translate(pos)
            * glm::rotate(rot,glm::vec3(0.0f,0.0f,1.0f))
            * glm::scale(vec3(scale.x,scale.y,1.0f));

 }

void SkeletonPicture::draw(const glm::mat4& mvp,const Color& ambientLight) const{
    pres->draw(mvp*drawMatrix,ambientLight);
}

void SkeletonPicture::refreshDrawMatrix(){
    drawMatrix= parent->getJoint()->getAnimationMatrix()*initMatrix;
}

