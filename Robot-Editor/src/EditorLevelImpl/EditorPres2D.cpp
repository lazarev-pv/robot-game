
#include <EditorLevel.h>
#include <EditorLevelImpl/EditorPres2D.h>
#include <EditorLevelImpl/EditorGeometryObject.h>
#include <EditorLevelImpl/Layer.h>
#include <EditorLevelImpl/EditorConstants.h>

#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/UserInterface/ContextMenu.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/Render/Sprites/PatternSprite.h>

using namespace game;
using namespace std;
using namespace glm;

static inline vec3 fullPos(const glm::vec3& p, OperableObject *lo){
    return lo!=nullptr?p+lo->getPosition():p;
}

static inline vec3 offsetPos(const glm::vec3& p, OperableObject *lo){
return lo!=nullptr?p-lo->getPosition():p;
}

EditorPres2D::EditorPres2D(
        const glm::vec3& offset,
        const std::string& spriteName,
        const glm::vec2& scale,
        EditorFixtureObject* levelObject,
        Layer* layer)

	:BoxAreaObject(fullPos(offset,levelObject),vec2(1,1),EditorConstants::PRES2D,0,"",layer),
    levelObject(levelObject),
    sprite(Game::getResources()->createPatternSprite(spriteName,1.0,1.0)),
    spread(false),
    scale(scale){

    setSize(vec2(sprite->getHW()*scale.x, sprite->getHH()*scale.y));
}

EditorPres2D::EditorPres2D(ArtType* art, EditorFixtureObject * obj, Layer* layer)
    :BoxAreaObject(fullPos(parse(art->pos),obj),vec2(1,1),EditorConstants::PRES2D,art->zOrder,art->name,layer),
     levelObject(obj),
     scale(parse(art->scale)),
     color(art->color?parse(art->color):Color::White){

    if(art->repeat!=nullptr){
        sprite = Game::getResources()->createPatternSprite(art->image->name,art->repeat->x,art->repeat->y);
    }else{
        sprite = Game::getResources()->createPatternSprite(art->image->name,1.0,1.0);
    }
    setSize(vec2(sprite->getHW()*art->scale->x,
            sprite->getHH()*art->scale->y));
    setRotation(art->rotation);
}

EditorPres2D::~EditorPres2D(){
	if(levelObject!=nullptr)
		levelObject->removePresentation(this);
}

void EditorPres2D::setScale(const glm::vec2& scale){
	this->scale=scale;
    setSize(vec2(scale.x*sprite->getHW(),scale.y*sprite->getHH()));
	fireEvent("");
}

void EditorPres2D::spreadTexture(const glm::vec2& sizeDelta){
    vec2 scale = getScale();
	sprite->spread(vec2(sizeDelta.x/scale.x,sizeDelta.y/scale.y));
}

EditorFixtureObject* EditorPres2D::getParent(){
	return levelObject;
}

void EditorPres2D::draw(CameraCHandle camera){
    sprite->draw(createMatrix(getPosition(),getRotation(),scale,camera),
                EditorLevel::instance()->getAmbientLight()*color);
    if (EditorLevel::instance()->isSelectable(EditorConstants::PRES2D))
        BoxAreaObject::draw(camera);
}

glm::vec2 EditorPres2D::getScale() const{
    return scale;
}

ArtType* EditorPres2D::save2ArtType() const {
    ArtType* s = new ArtType();
    s->image = new Reference();
    s->image->name = sprite->getName();
    s->pos = create3D(offsetPos(getPosition(),levelObject));
    s->zOrder = getZOrder();
    s->name = getName();
    s->scale = new Point2D();
    s->scale = create2D(getScale());
    s->rotation = getRotation();
    s->color = color.toColorType();
    if(sprite->getXRepeats()!=1.0 || sprite->getYRepeats()!=1.0){//ну а вдруг
        Point2D* p = new Point2D();
        p->x = sprite->getXRepeats();
        p->y = sprite->getYRepeats();
        s->repeat = p;
    }
    return s;
}

const std::string& EditorPres2D::getSpriteName() const{
    return sprite->getName();
}

void EditorPres2D::save2Layer(WorldLayers* layer) const{
    layer->art.push_back(save2ArtType());
}

void EditorPres2D::setSelected(bool isSelected){
    BoxAreaObject::setSelected(isSelected);
    if(isSelected)
        EditorLevel::instance()->selectPres2D(this);
}

OperableObject* EditorPres2D::clone(){
    EditorPres2D* ep = new EditorPres2D(getPosition(), sprite->getName(), getScale(),nullptr, getLayer());
    ep->setRotation(getRotation());
    ep->Move(vec3(0.5,0.5,0));
    EditorLevel::instance()->add2Pres(ep);
	return ep;
}

void EditorPres2D::Move(const glm::vec3& delta){
    vec2 oldSize = getSize();
    BoxAreaObject::Move(delta);
    vec2 deltaSize = getSize() - oldSize;

    if(isBorderMoving()) {
        if (spread) {
            sprite->spread(vec2(deltaSize.x/scale.x,deltaSize.y/scale.y));
        } else {
            scale += vec2(deltaSize.x / sprite->getHW(), deltaSize.y / sprite->getHH());
        }
    }
}