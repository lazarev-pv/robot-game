
#include <EditorLevel.h>
#include <EditorLevelImpl/EventObject.h>
#include <EditorLevelImpl/EditorConstants.h>
#include <EditorLevelImpl/EditorGeometryObject.h>
#include <EditorLevelImpl/Layer.h>
#include <EditorLevel.h>
#include <EditorLevelImpl/EditorConstants.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Game.h>

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

EventObject::EventObject(const glm::vec2& pos,
        const std::string& onEnter,
        const std::string& onLeave,
        const glm::vec2& size,
        bool infinite,
        const std::string& name)
	:BoxAreaObject(vec3(pos,0),size,EditorConstants::EVENT,0,name),
     infinite(false),
     onEnter(onEnter),
     onLeave(onLeave),
    text(Game::getLookAndFeel().font,name){
}

EventObject::EventObject(EventType* iter):
    EventObject(parse(iter->pos),
            iter->onEnter,
            iter->onLeave,
            parse(iter->size),
            iter->infinite,
            iter->name){}

EventObject::~EventObject(){
}

void EventObject::draw(CameraCHandle camera){
    Game::getPrimitiveDrawer()->drawFilledRectangle(
            createMatrix(getPosition(),getSize(),camera),
            Color(Color(Color::Red).setAlpha(0.5f)));
    if(EditorLevel::instance()->isSelectable(EditorConstants::EVENT)) {
        BoxAreaObject::draw(camera);
    }
    text.draw(getPosition(), Color::White, camera);
}

void EventObject::save2World(WorldType* world){
    EventType* et = new EventType();
    et->pos = create2D(getPosition());
    et->size = create2D(getSize());
    et->onEnter = onEnter;
    et->onLeave = onLeave;
    et->name = getName();
    et->infinite = infinite;
    world->event.push_back(et);
}

void EventObject::setSelected(bool isSelected){
    BoxAreaObject::setSelected(isSelected);
	if(isSelected)
        EditorLevel::instance()->selectEvent(this);
}

OperableObject* EventObject::clone(){
	EventObject* b = new EventObject(
            vec2(getPosition().x,getPosition().y),
            onEnter,
            onLeave,
            getSize(),
            infinite,
            "");

    b->Move(vec3(1,0,0));
    EditorLevel::instance()->add(b);
    return b;
}
