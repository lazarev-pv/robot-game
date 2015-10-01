#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/transform.hpp>

#include <ChedBone.h>
#include <ChedSlot.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChedLevel.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <UI/AnimationControl.h>
#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <iostream>
#include <ChibiEngine/SkeletonAnimation/Skeleton.h>
#include <ChibiEngine/SkeletonAnimation/SkeletonJoint.h>
#include <ChibiEngine/Common/StringUtils.h>

using namespace game;
using namespace std;
using namespace glm;

int ChedBone::boneCount = 0;

quat RotationBetweenVectors(vec3 start, vec3 dest){
    start = normalize(start);
    dest = normalize(dest);

    float cosTheta = dot(start, dest);
    vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f){
        rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
        if (length2(rotationAxis) < 0.01 ){
            rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);
        }
        rotationAxis = normalize(rotationAxis);
        return angleAxis(float(M_PI), rotationAxis);
    }

    rotationAxis = cross(start, dest);

    float s = sqrt( (1+cosTheta)*2 );
    float invs = 1 / s;

    return quat(
            s * 0.5f,
            rotationAxis.x * invs,
            rotationAxis.y * invs,
            rotationAxis.z * invs
    );

}

glm::vec3 ChedBone::getBaseEnd() const{
    return joint->getBaseEnd();
}
glm::vec2 ChedBone::getPosition() const{
    return toVec2(joint->getBaseBegin()+joint->getAnimationPositionOffset())+currentDirection*(joint->getLength()*0.5f);
}

void ChedBone::calcBeginAndEnd(glm::vec2* beg, glm::vec2* end) const{
    vec4 _begin(0.0f,-joint->getLength()*0.5,0,1);
    vec4 _end(0.0f,joint->getLength()*0.5,0,1);

    mat4 m = joint->getAnimationMatrix();
    *beg = toVec2(m*_begin);
    *end= toVec2(m*_end);
}

ChedBone::ChedBone(ChedLevel* level,
            Skeleton* skeleton,
            SkeletonJoint* joint,
            const std::string& name,
            const Color& color):
    joint(joint),
    obj(Game::getResources()->findSprite("bone")),
    name(name),
    color(color),
    initDir(2.0f*joint->getBoneVector()){

    boneCount++;
    skeletonChanged(level);
}


ChedBone::ChedBone(ChedLevel* level, Skeleton* skeleton, BoneType* data):
        ChedBone(level,
                 skeleton,
                 skeleton->findJoint(data->id),
                 data->name,
                 parse(data->color,Color::White)){
}

BoneType* ChedBone::save(){
    BoneType* bt = new BoneType();

    bt->begin = create2D(joint->getBaseBegin());
    bt->end = create2D(getBaseEnd());
    bt->id = static_cast<int>(joint->getId());
    bt->color = color.toColorType();
    SkeletonJoint* parent = joint->getParent();
    if(parent){
        ChedBone* chedBoneParent = ChedLevel::instance()->findBoneById(parent->getId());
        bt->parent = chedBoneParent->getName();
    }
    bt->name = name;

    vector<ChedSlot*> slots = ChedLevel::instance()->findSlotByBone(joint->getId());
    for(ChedSlot* slot : slots){
        bt->slot.push_back(slot->save());
    }

    return bt;
}

void ChedBone::draw(CameraCHandle camera){
    ChedObject::draw(camera);
    // Draw ChedBone
    const Color& color = highligthed?Color::Yellow:
            selected?Color::Green:
                    this->color;

    float _scale = hh/obj->getHH();

    mat4 tmp = Game::getCamera()->getMVPMatrix()
            * joint->getAnimationMatrix()
            * glm::scale(vec3(_scale,_scale,1.0f));

    obj->draw(tmp, color);
}

void ChedBone::setSelected(bool flag){
    ChedObject::setSelected(flag);
    if(flag){
        ChedLevel::instance()->getAnimationControl()->selectBone(getName());
    }
}
/*
void ChedBone::recalcSupportData(){
    *//*pos.x = 0.5f*(begin.x+end.x);
    pos.y = 0.5f*(begin.y+end.y);*//*

    currentRotation = getAngle(glm::normalize(toVec2(vec4(0,1,0,1) * joint->getAnimationMatrix())));
    currentDirection = vec2(currentRotation>0?cos(currentRotation):-cos(currentRotation),sin(currentRotation));

    hh = joint->getLength();//glm::distance(end,begin);
    hw = hh*obj->getHW()/obj->getHH()*0.5f;
    if(! hh>0.001){
        hh=0.0015;
    }
    *//*if(begin.x<end.x){
        rotation =-glm::acos((end.y-begin.y)/hh);
    }else{
        rotation = glm::acos((end.y-begin.y)/hh);
    }*//*
    hh*=0.5;

    calcBoundVerts(joint->getAnimationMatrix());

}*/

const std::string&ChedBone::getName() const{
    return name;
}

void ChedBone::setName(const std::string& name){
    this->name = name;
}

Animation::BoneId ChedBone::getId() const{
    return joint->getId();
}

float ChedBone::getRotation() const{
    return currentRotation;
}

glm::vec2 ChedBone::getScale() const{
    return toVec2(joint->getCurrentScale());
}

glm::vec2 ChedBone::calcOffset(const glm::vec2& delta) const{
    return delta;
}

glm::vec2 ChedBone::calcScale(const glm::vec2& delta) const{
    return vec2(1,1)+delta/vec2(hw,hh);
}

glm::quat ChedBone::calcRotation(const glm::vec2& currPnt, const glm::vec2& delta) const{
    vec4 pos = joint->getAnimationMatrix() * vec4(0.0f,-joint->getLength()*0.5,0,1);
    float angle = getAngle(currPnt, delta, toVec2(pos));
    return angleAxis(angle, vec3(0.0f,0.0f,1.0f));
}

glm::vec2 ChedBone::getBegin() const{
    return toVec2(joint->getAnimationMatrix()*vec4(0.0f,-hh,0.0f,1.0f));
}

const glm::vec2 ChedBone::getDirection() const{
    return currentDirection;
}


float game::getAngle(const glm::vec2& currPnt, const glm::vec2& delta, const glm::vec2& pos){

    vec2 b = currPnt - delta - pos;
    vec2 e = currPnt         - pos;

    float norm =  length(b)*length(e);
    if(norm<=std::numeric_limits<float>::epsilon()) {
        return 0.0f;
    }

    float cosAngl = dot(b,e)/norm;
    if(cosAngl>1.0f){
        cosAngl=1.0f;
    }
    if(cosAngl<-1.0f){
        cosAngl=-1.0f;
    }

    if(cross(vec3(b,0),vec3(e,0)).z>0){
        return fabs(acos(cosAngl));
    }

    return -fabs(acos(cosAngl));
}

void ChedBone::scale(const glm::vec2& currPnt, const glm::vec2& delta){
    joint->setBase(joint->getBaseBegin(),joint->getBaseEnd()+vec3(delta,0));
    ChedLevel::instance()->getSkeleton()->refreshAnimation();

    skeletonChanged(ChedLevel::instance());
}

void ChedBone::rotate(const glm::vec2 &currPnt, const glm::vec2 &delta){
    vec4 baseBeginH = vec4(joint->getBaseBegin(),1.0f);
    float angle = getAngle(currPnt, delta, toVec2(baseBeginH));
    Skeleton* skeleton = ChedLevel::instance()->getSkeleton();

    vec3 rotationPoint = joint->getBaseBegin();
    set<Animation::BoneId> children;
    skeleton->findBoneChildren(children, joint->getId());

    ChedLevel* level = ChedLevel::instance();
    for(auto& boneId : children){
        SkeletonJoint* currentJoint = skeleton->findJoint(boneId);

        mat4 transMat = glm::translate(rotationPoint)
                * glm::rotate(angle, vec3(0.0f,0.0f,1.0f))
                * glm::translate(-rotationPoint);

        vec3 newEnd = toVec3(transMat * vec4(currentJoint->getBaseEnd(),1.0f));
        vec3 newBegin = toVec3(transMat * vec4(currentJoint->getBaseBegin(),1.0f));

        currentJoint->setBase(newBegin, newEnd);

        // Skeleton Changed stuff
        level->findBoneById(boneId)->skeletonChanged(ChedLevel::instance());
    }

    skeleton->refreshAnimation();
    skeletonChanged(ChedLevel::instance());
}

void ChedBone::move(const glm::vec2 currPnt, const glm::vec2 &delta){
    set<Animation::BoneId > children;
    Skeleton* skeleton = ChedLevel::instance()->getSkeleton();
    skeleton->findBoneChildren(children, joint->getId());
    ChedLevel* level = ChedLevel::instance();
    for(auto id:children){
        SkeletonJoint* joint = skeleton->findJoint(id);
        joint->setBaseBegin(joint->getBaseBegin()+vec3(delta,0));

        // Skeleton Changed stuff
        level->findBoneById(id)->skeletonChanged(ChedLevel::instance());
    }
    skeleton->refreshAnimation();
}

void ChedBone::skeletonChanged(ChedLevel* level){
    vec4 p = joint->getAnimationMatrix()*vec4(0,1,0,0);
    currentDirection = glm::normalize(vec2(p.x,p.y));
    currentRotation = getAngle(currentDirection);

    hh = joint->getLength();
    hw = hh*obj->getHW()/obj->getHH()*0.5f;
    if(! hh>0.001){
        hh=0.0015;
    }
    hh*=0.5;

    calcBoundVerts(joint->getAnimationMatrix());
    fireEvent("");

    for(auto iter : level->findSlotByBone(joint->getId())){
        iter->skeletonChanged(level);
    }
}
