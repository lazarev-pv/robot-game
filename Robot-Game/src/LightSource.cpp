#include <LightSource.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>

using namespace game;
using namespace glm;
using namespace std;

LightSource::LightSource(int uid, Layer* layer, int zOrder, const std::string& name):
        LevelObject(uid,layer,zOrder,name){
}

LightSource* LightSource::create(int uid, const LightType* l,Layer* layer){
    if(l->choiceValue.spot!=nullptr){
        return new SpotLightSource(uid,l,layer);
    }else if(l->choiceValue.directional!=nullptr){
        return new DirLightSource(uid,l,layer);
    }
    return nullptr;
}

SpotLightSource::SpotLightSource(int uid, const LightType* l,Layer* layer):
        LightSource(uid,layer,0,l->name),
        pres(Game::getResources()->findSprite("light")){

    color = Color(l->r,l->g,l->b,l->a);
    SpotLightType* type = l->choiceValue.spot;
    intensity = type->i;
    pos=vec3(type->pos->x,type->pos->y, type->pos->z);
    size = vec2(type->hw, type->hh);
    setEnabled(l->enable);
}

void SpotLightSource::draw(CameraCHandle camera){
    if(isEnabled()) {
        vec2 scale(size.x / pres->getHW(), size.y / pres->getHH());
        pres->draw(createMatrix(pos, scale, camera), color * intensity);
    }
}


DirLightSource::DirLightSource(int uid, const LightType* l,Layer* layer):
        LightSource(uid,layer,0,l->name){

    color = Color(l->r,l->g,l->b,l->a);
    vec3 pnts[4];
    DirLightType* d = l->choiceValue.directional;
    pnts[0]=vec3(d->lb->x,d->lb->y,d->lb->z);
    pnts[1]=vec3(d->lt->x,d->lt->y,d->lt->z);
    pnts[2]=vec3(d->rt->x,d->rt->y,d->rt->z);
    pnts[3]=vec3(d->rb->x,d->rb->y,d->rb->z);

    pres = new ArbitrarySprite(pnts,Game::getResources()->findSpriteInfo("light_dir"));
    setEnabled(l->enable);
}


void DirLightSource::draw(CameraCHandle camera){
    if(isEnabled())
        pres->draw(camera->getMVPMatrix(),color);
}

DirLightSource::~DirLightSource(){
    delete pres;
}


glm::vec3 DirLightSource::getPosition(){
    return vec3(0,0,0);
}
