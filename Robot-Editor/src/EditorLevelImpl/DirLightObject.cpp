#include <EditorLevelImpl/DirLightObject.h>
#include <EditorLevelImpl/EditorGeometryObject.h>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Resource/Resources.h>
#include <EditorLevel.h>
#include <ChibiEngine/Render/Sprites/ArbitrarySprite.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Common/MathUtils.h>
#include <ChibiEngine/Common/ParserUtils.h>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#include <EditorLevelImpl/EditorConstants.h>
#include <ChibiEngine/Common/MathUtils.h>

using namespace game;
using namespace glm;
using namespace std;

static const float HW=0.1;

DirLightObject::DirLightObject(const glm::vec3& offset, const glm::vec2& size):
        OperableObject(0),
        moveIndex(-1),
        color(Color::White){

    pnts[0]=offset+vec3(-HW,-HW,0);
    pnts[1]=offset+vec3(-HW,HW,0);
    pnts[2]=offset+vec3(HW,HW,0);
    pnts[3]=offset+vec3(HW,-HW,0);

    pres = new ArbitrarySprite(pnts,Game::getResources()->findSpriteInfo("light_dir"));
}

DirLightObject::DirLightObject(const LightType* l)
:OperableObject(0,l->name),
 moveIndex(-1){

    DirLightType* d = l->choiceValue.directional;
    pnts[0]=vec3(d->lb->x,d->lb->y,d->lb->z);
    pnts[1]=vec3(d->lt->x,d->lt->y,d->lt->z);
    pnts[2]=vec3(d->rt->x,d->rt->y,d->rt->z);
    pnts[3]=vec3(d->rb->x,d->rb->y,d->rb->z);;

    color.setRed(l->r);
    color.setGreen(l->g);
    color.setBlue(l->b);
    color.setAlpha(l->a);
    setEnable(l->enable);
    pres = new ArbitrarySprite(pnts,Game::getResources()->findSpriteInfo("light_dir"));
}

void DirLightObject::Move(vec3 const &delta) {
    if(moveIndex>=0){
        pnts[moveIndex]+=delta;
    }
    pres->changeVecs(pnts);
}

void DirLightObject::grab(vec2 const &pnt) {
    moveIndex=-1;
    for(int i=0;i<4;++i){
        if(game::contains(vec2(pnts[i].x,pnts[i].y),HW,HW,pnt)){
            moveIndex=i;
            return;
        }
    }
}

DirLightObject::~DirLightObject(){
    delete pres;
}

void DirLightObject::release() {
}

void DirLightObject::setSelected(bool _isSelected){
    OperableObject::setSelected(_isSelected);
    if(isSelected())
        EditorLevel::instance()->selectDirLight(this);
}

void DirLightObject::draw(CameraCHandle camera) {
    if(EditorLevel::instance()->isSelectable(EditorConstants::DIR_LIGHT)) {
        const PrimitiveDrawer *drawer = Game::getPrimitiveDrawer();
        for (int i = 0; i < 4; ++i) {
            drawer->drawRectangleBorder(
                    createMatrix(pnts[i], vec2(HW, HW),camera),
                    Color::White);
        }
    }
    pres->draw(camera->getMVPMatrix(),color);
}

OperableObject *DirLightObject::containsPoint(vec2 const &pnt) {
    if(!EditorLevel::instance()->isSelectable(EditorConstants::DIR_LIGHT))
        return nullptr;

    for(int i=0;i<4;++i){
        if(game::contains(vec2(pnts[i].x,pnts[i].y),HW,HW,pnt)){
            return this;
        }
    }
    return nullptr;
}

void DirLightObject::save2World(WorldType *) {

}

void DirLightObject::viewContextMenu(EditorLevel *, vec2 const &) {

}

OperableObject *DirLightObject::clone() {
    return this;
}

void DirLightObject::setPosition(vec3 const &position) {
    for(int i=0; i<4; ++i){
        vec3 v = position-getPosition();
        pnts[i]+=v;
    }
}

glm::vec3 DirLightObject::getPosition() const{
    vec3 v(0,0,0);
    for(int i=0;i<4;++i){
        v+=pnts[i];
    }
    v/=4.0f;
    return v;
}

void DirLightObject::save2Layer(WorldLayers* layer) const{
    LightType* res = new LightType();
    DirLightType* d = new DirLightType();
    res->choiceValue.directional = d;

    d->lb = create3D(pnts[0]);
    d->lt = create3D(pnts[1]);;
    d->rt = create3D(pnts[2]);;
    d->rb = create3D(pnts[3]);;

    res->r = color.toVector().r;
    res->g = color.toVector().g;
    res->b = color.toVector().b;
    res->a = color.toVector().a;
    res->name = getName();
    res->enable = isEnable();
    layer->light->light.push_back(res);
}

