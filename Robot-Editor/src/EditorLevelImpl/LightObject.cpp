#include <EditorLevelImpl/LightObject.h>
#include <EditorLevelImpl/EditorGeometryObject.h>
#include <ChibiEngine/Platform/Platform.h>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Resource/Resources.h>
#include <EditorLevel.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <EditorLevelImpl/EditorConstants.h>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

using namespace game;
using namespace glm;
using namespace std;

SpotLightObject::SpotLightObject(const glm::vec3& offset, const glm::vec2& size):
        BoxAreaObject(offset,size,EditorConstants::SPOT_LIGHT),
        pres(Game::getResources()->findSprite("light")),
        color(Color::White),
        intensity(1.0f){
}

SpotLightObject::SpotLightObject(const LightType* l):
        BoxAreaObject(parse(l->choiceValue.spot->pos),
        vec2(l->choiceValue.spot->hw,
             l->choiceValue.spot->hh),EditorConstants::SPOT_LIGHT,0,l->name),
 pres(Game::getResources()->findSprite("light")),
 color(l->r,l->g,l->b,l->a),
 intensity(l->choiceValue.spot->i){
    setEnable(l->enable);
}

void SpotLightObject::draw(CameraCHandle camera) {
    pres->draw(createMatrix(getPosition(), getSize()/vec2(pres->getHW(),pres->getHH()),camera), color*intensity);
    if(EditorLevel::instance()->isSelectable(EditorConstants::SPOT_LIGHT))
        BoxAreaObject::draw(camera);
}

OperableObject *SpotLightObject::clone() {
    return this;
}

void SpotLightObject::save2Layer(WorldLayers* layer) const{
    LightType* res = new LightType();
    SpotLightType* s =new SpotLightType();
    res->choiceValue.spot = s;
    s->pos = create3D(getPosition());
    res->name = getName();
    res->r = color.toVector().r;
    res->g = color.toVector().g;
    res->b = color.toVector().b;
    res->a = color.toVector().a;
    res->enable = isEnable();
    s->i = intensity;
    s->hw = getSize().x;
    s->hh = getSize().y;
    layer->light->light.push_back(res);
}


void SpotLightObject::setSelected(bool isSelected){
    BoxAreaObject::setSelected(isSelected);
    if(isSelected)
        EditorLevel::instance()->selectSpotLight(this);
}
