#include <ChibiEngine/SkeletonAnimation/SkeletonJoint.h>

#define GLM_FORCE_RADIANS

#include <glm/gtx/norm.hpp>
#include <glm/gtx/transform.hpp>

#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/Common/ParserUtils.h>

using namespace game;
using namespace glm;
using namespace std;

SkeletonJoint::SkeletonJoint(const glm::vec2& begin, const glm::vec2& end):
	parent(nullptr){

	setBase(vec3(begin,0), vec3(end,0));
	prepareForAnimation();
}

SkeletonJoint::SkeletonJoint(BoneType* xml):
	SkeletonJoint(parse(xml->begin),
	parse(xml->end)){
	id = static_cast<Animation::BoneId >(xml->id);
}

void SkeletonJoint::setBase(const glm::vec3& begin, const glm::vec3 end){
	baseBegin = begin;
	boneVector = 0.5f*(end-begin);
	length = glm::distance(begin,end);

	float currentRotation = begin.x<end.x?
			-glm::acos((end.y-begin.y)/length):
			+glm::acos((end.y-begin.y)/length);

	setBaseRotate(currentRotation);
}

void SkeletonJoint::setBaseRotate(float angle) {
	baseRotationTransform = glm::rotate(
			glm::mat4(1.0),
			angle,
			glm::vec3(0.0f,0.0f,1.0f));
}

void SkeletonJoint::applyTransition(const Animation::TimeStamp& frame){
	const SkeletonJoint* current = this;

    // Scale
    {
        auto tran = frame.find(current->id);
        if(tran!=frame.transitions.end()){
            animationScale.x*=tran->scale.x;
            animationScale.y*=tran->scale.y;
        }
    }

	//Rotation
	while(current!=nullptr){
		Animation::TimeStamp::CInterTransIt tran = frame.find(current->id);
		if(tran==frame.transitions.end()){
			current=current->parent;
			continue;
		}

		mat4 transMat = translate(current->baseBegin)
			* mat4_cast(tran->rotation)
			* translate(-current->baseBegin);

		animationRotationTransform = transMat*animationRotationTransform;
		current=current->parent;
	}

	current = this;

    // move
	while(current!=nullptr){
		Animation::TimeStamp::CInterTransIt tran = frame.find(current->id);
		if(tran==frame.transitions.end()){
			current=current->parent;
			continue;
		}

		animationPositionOffset +=vec3(tran->offset,0);
		current=current->parent;
	}
}

glm::mat4 SkeletonJoint::getAnimationMatrix() const{
	return glm::translate(animationPositionOffset)
            * animationRotationTransform
            * scale(mat4(),animationScale)
            * glm::translate(baseBegin + boneVector)
            * baseRotationTransform;
}

glm::vec3 SkeletonJoint::getBaseEnd() const{
	return baseBegin + boneVector*2.0f;
}
