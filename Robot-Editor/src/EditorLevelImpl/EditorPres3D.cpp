
#include <EditorLevel.h>
#include <EditorLevelImpl/EditorPres3D.h>
#include <EditorLevelImpl/EditorGeometryObject.h>
#include <EditorLevelImpl/Layer.h>
#include <EditorLevelImpl/EditorConstants.h>

#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Render/3D/Block.h>
#include <ChibiEngine/UserInterface/ContextMenu.h>
#include <ChibiEngine/Common/MathUtils.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/XMLParser/WorldParser.h>

using namespace game;
using namespace std;
using namespace glm;

EditorPres3D::EditorPres3D(const glm::vec3& pos,
        const Texture* tex,
        Layer* layer,
        const glm::vec3& size,
        const glm::vec2& texes,
        const std::string& name,
        const Color& color)
	:BoxAreaObject(pos, vec2(size.x,size.y),EditorConstants::PRES3D,0,name,layer),
	 sprite(new Block(tex->getUID(),texes)),
    tex(tex),
    hd(size.z),
    spread(false),
    color(color){
}

EditorPres3D::EditorPres3D(Box3DType* iter, Layer* layer):
    EditorPres3D(parse(iter->pos),
            Game::getResources()->findTexture(iter->texture),
            layer,
            parse(iter->size),
            parse(iter->tex),
            iter->name,
            iter->color?parse(iter->color):Color::White){
}

EditorPres3D::~EditorPres3D(){
	delete sprite;
}

void EditorPres3D::save2Layer(WorldLayers* layer) const{
    Box3DType* art = new Box3DType();
    art->pos=create3D(getPosition());
    art->size = create3D(vec3(getSize(),hd));
    art->texture = tex->getName();
    art->tex = create2D(sprite->getTexes());
    art->name = getName();
    art->color = color.toColorType();
    layer->objs3d->box3D.push_back(art);
}

void EditorPres3D::draw(CameraCHandle camera){
    sprite->draw(getPosition(), vec3(getSize(),hd), camera, EditorLevel::instance()->getAmbientLight()*color);
    if(EditorLevel::instance()->isSelectable(EditorConstants::PRES3D))
        BoxAreaObject::draw(camera);
}

void EditorPres3D::setSelected(bool isSelected){
    BoxAreaObject::setSelected(isSelected);
	if(BoxAreaObject::isSelected())
        EditorLevel::instance()->selectPres3D(this);
}

void EditorPres3D::setTexels(const glm::vec2& tex){
    sprite->setTexes(tex);
}

OperableObject*EditorPres3D::clone(){
	EditorPres3D * b = new EditorPres3D(getPosition(),tex,getLayer(),vec3(getSize(),hd),sprite->getTexes());
    b->Move(vec3(1,0,0));
    EditorLevel::instance()->add(b);
    return b;
}

const glm::vec2&EditorPres3D::getTexels() const{
    return sprite->getTexes();
}

