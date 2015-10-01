#include <LevelObjects/Object3D.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Render/3D/Block.h>
#include <ChibiEngine/Common/ParserUtils.h>

using namespace std;
using namespace game;
using namespace glm;
using namespace rapidxml;

Object3D::Object3D(int uid, Box3DType* data, Layer* layer)
:LevelObject(uid,layer, 1,data->name),
 pos(parse(data->pos)),
 size(parse(data->size)),
 presentation(new Block(
         Game::getResources()->findTexture(data->texture)->getUID(),
         parse(data->tex))),
 color(game::parse(data->color))
 {
 }


void Object3D::draw(CameraCHandle camera,const Color& ambientLight){
    if(isEnabled()) {
        presentation->draw(pos, size, camera, ambientLight*color);
    }
}

Object3D::~Object3D(){
    delete presentation;
}
