#include <EditorLevelImpl/NamedPointObject.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Common/MathUtils.h>
#include <EditorLevel.h>
#include <EditorLevelImpl/EditorConstants.h>

using namespace game;
using namespace std;
using namespace glm;

static const float WIDTH = 0.05;


NamedPointObject::NamedPointObject(const glm::vec3& pos, const std::string& pointText,
        const std::string& className)
	:PointObject(pos,pointText),
	 text(Game::getLookAndFeel().font,pointText),
    className(className){
}


NamedPointObject::NamedPointObject(NamedPoint2D* pnt)
    :NamedPointObject(vec3(pnt->x,pnt->y,0),pnt->name){
}

NamedPointObject::~NamedPointObject(){}

void NamedPointObject::draw(CameraCHandle camera){
    if(EditorLevel::instance()->isSelectable(className)) {
        PointObject::draw(camera);
        text.draw(getPosition() + vec3(0, WIDTH * 2, 0), Color::Red, camera);
    }
}

void NamedPointObject::setName(const std::string& name){
    PointObject::setName(name);
    text.setText(name);
}

void NamedPointObject::save2World(WorldType* world){
	NamedPoint2D* pnt = new NamedPoint2D();
	pnt->name = getName();
    vec3 p = getPosition();
	pnt->x = p.x;
	pnt->y = p.y;
	world->points.push_back(pnt);
}

OperableObject* NamedPointObject::clone(){
    NamedPointObject* npo = new NamedPointObject(getPosition(),getName());
    npo->Move(vec3(0.5,0,0));
    EditorLevel::instance()->add2Others(npo);
    return npo;
}

void NamedPointObject::setSelected(bool isSelected){
    PointObject::setSelected(isSelected);
    if(isSelected)
        EditorLevel::instance()->selectPoint(this);
}

OperableObject* NamedPointObject::containsPoint(const glm::vec2& pnt){
    if(!EditorLevel::instance()->isSelectable(className))
        return nullptr;
    return PointObject::containsPoint(pnt);
}
