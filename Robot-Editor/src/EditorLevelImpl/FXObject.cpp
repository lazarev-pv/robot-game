
#include <EditorLevelImpl/FXObject.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Render/Particles/ParticleSystem.h>
#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/Common/MathUtils.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <EditorLevelImpl/EditorConstants.h>
#include <EditorLevel.h>

using namespace game;
using namespace std;
using namespace glm;

static const float DIR_PNT_HW = 0.02;

FXObject::FXObject(FXType* fxType,Layer* layer)
    : FXObject(fxType->effect, EffectParams(fxType),fxType->zOrder,fxType->name,layer){
}

FXObject::FXObject(std::string effectName, const EffectParams& p,
        int zOrder,const std::string& name, Layer* layer):
        BoxAreaObject(p.pos,p.size,EditorConstants::FX_OBJECT,zOrder,name,layer),
    dirPnt(vec3(p.dir,0), "dir"),
    text(Game::getLookAndFeel().font,effectName),
    effect(Game::getParticleSystem()->createEffect(effectName, p))
{
    dirPnt.Move(vec3(getPosition().x, getPosition().y-p.size.y, getPosition().z));
    effect.setDirection(p.dir);
}

OperableObject*FXObject::clone(){
	FXObject * fx = new FXObject(
            effect.getTemplate()->getName(),
            EffectParams(getPosition()+vec3(1.0f,0.0f,0.0),
                    effect.getSize(),
                    effect.getDirection(),
                    effect.getColor())
            );

    EditorLevel::instance()->add(fx);
    return fx;
}

void FXObject::Move(const glm::vec3& delta){
    BoxAreaObject::Move(delta);

	effect.setPosition(getPosition());
	effect.setSize(getSize());
    dirPnt.Move(delta);
	fireEvent(EventSupportable::MOVE_EVENT);
}


void FXObject::draw(CameraCHandle camera){

    vec3 dirOffset=vec3(getPosition().x, getPosition().y-getSize().y, getPosition().z);
    {
        vec3 p = dirPnt.getPosition() - dirOffset;
        effect.setDirection(vec2(p.x,p.y));
        effect.draw(camera, Game::getLevelClock()->getTime() * 0.001f + 100.0f);
    }
    dirPnt.draw(camera);
    Game::getPrimitiveDrawer()->drawSimpleArrow(dirOffset, dirPnt.getPosition(), Color::Green, camera);

    if(EditorLevel::instance()->isSelectable(EditorConstants::PRES3D)) {
        BoxAreaObject::draw(camera);
        text.draw(getPosition(), Color::Yellow, camera);
    }
}

OperableObject*FXObject::containsPoint(const glm::vec2& pnt){
    {
        OperableObject *obj = dirPnt.containsPoint(pnt);
        if (obj != nullptr) {
            return obj;
        }
    }
    return BoxAreaObject::containsPoint(pnt);
}

void FXObject::setPosition(const glm::vec3& position){
    BoxAreaObject::setPosition(position);
	effect.setPosition(position);
}

void FXObject::save2Layer(WorldLayers* layer) const{
    FXType* data = new FXType();
    data->x=getPosition().x;
    data->y=getPosition().y;
    data->hw=getSize().x;
    data->hh=getSize().y;
    data->zOrder = getZOrder();
    data->effect=effect.getTemplate()->getName();
    data->color=effect.getColor().toColorType();
    data->name=getName();
    data->dir = create2D(effect.getDirection());
    layer->fxes->fx.push_back(data);
}

void FXObject::setColor(const Color& color){
    effect.setColor(color);
}

const Color& FXObject::getColor(){
    return effect.getColor();
}

void FXObject::setSelected(bool _isSelected){
    BoxAreaObject::setSelected(_isSelected);
    if(isSelected())
        EditorLevel::instance()->selectFxObject(this);
}