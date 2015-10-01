#include <UI/TransformKey.h>
#include <UI/TimeLine.h>
#include <UI/AnimationControl.h>
#include <ChedLevel.h>
#include <ChibiEngine/Common/MathUtils.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>

using namespace game;
using namespace std;
using namespace glm;

static const float KEY_HW = 0.006;
static const float KEY_HH_PERCENT = 0.8f;

glm::vec2 TransformKey::getPosition(const glm::vec3& pos, const glm::vec2& size){
    return getPositionImpl(pos,size,time);
}

void TransformKey::draw(const glm::vec3& pos, const glm::vec2& size, const Color& color){
    TransformKey::drawImpl(pos,size,color,highlighted||selected,time);
}

glm::vec2 TransformKey::getPositionImpl(const glm::vec3& pos, const glm::vec2& size, int time){
    vec2 interval = ChedLevel::instance()->getAnimationControl()->getTimeLine()->getInterval();
    float posX = transformInterval<float>(interval.x, interval.y, -1.0f, 1.0f, time)*size.x+pos.x;
    return vec2(posX,pos.y);
}

void TransformKey::drawImpl(const glm::vec3& pos, const glm::vec2& size, const Color& color, bool highlighted, int time){
    vec3 p(getPositionImpl(pos,size, time),0);
    Game::getPrimitiveDrawer()->drawFilledRectangle(
            createMatrix(p, vec2(KEY_HW, size.y* KEY_HH_PERCENT)), color);
    Game::getPrimitiveDrawer()->drawRectangleBorder(
            createMatrix(p, vec2(KEY_HW, size.y* KEY_HH_PERCENT)),
            highlighted?Color::Aqua:Color::Black);
}
