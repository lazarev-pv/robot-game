#include <UI/TransformLabel.h>
#include <UI/TransformLine.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Common/MathUtils.h>
#include <iostream>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChedLevel.h>
#include <UI/AnimationControl.h>

using namespace game;
using namespace glm;
using namespace std;

TransformLabel::TransformLabel(
        const std::string &label,
        const std::string &icon,
        Color bgColor,
        TransformId transformId,
        const glm::vec3& pos,
        const glm::vec2& size)
    :obj(icon.size()>0?Game::getResources()->findSprite(icon):nullptr),
    text(Game::getLookAndFeel().font, label),
    bgColor(bgColor),
    transformId(transformId){

    setPosition(pos);
    setSize(size);
    setVisible(false);

}

void TransformLabel::draw() {
    UIElement::draw();

    Game::getPrimitiveDrawer()->drawFilledRectangle(
            createMatrix(getPosition(), getSize()),
            selected?rgba(104, 119, 119,255):bgColor);
    Game::getPrimitiveDrawer()->drawRectangleBorder(
            createMatrix(getPosition(), getSize()),
            Color::Black);

    float posx = getPosition().x;
    float posy = getPosition().y;
    float posz = getPosition().z;
    float titleX = posx - getSize().x;

    if(obj){ // draw icon
        float hh = getSize().y*0.95f;// text.getScreenHHeight()*1.0f;
        float hw = hh/Game::getScreen()->getAspectRatio();
        obj->draw(createMatrix(vec3(titleX+hw*1.5,posy,posz), 0, vec2(hw/obj->getHW(),hh/obj->getHH())));
        titleX+=hw*(1.5f+2.0f+0.5f);
        text.draw(vec3(titleX+text.getScreenHWidth(),posy,posz), Color::White);
    }else{
        text.draw(vec3(titleX+text.getScreenHWidth()*1.5f,posy,posz), Color::White);
    }
}

void TransformLabel::executeAction() {
    UIElement::executeAction();
    if(transformId!=NoTransform)
        ChedLevel::instance()->getAnimationControl()->selectTransform(transformId);
}
