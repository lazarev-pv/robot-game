#include <ChibiEngine/Game.h>
#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Render/Sprites/NineSliceSprite.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/UserInterface/Button.h>
#include <ChibiEngine/UserInterface/LookAndFeel.h>
#include <ChibiEngine/Script/Script.h>
#include <ftsizes.h>
#include <zlib.h>

using namespace game;
using namespace std;
using namespace glm;

const string Button::TYPE_NAME = "Button";


ImageButtonDraw::ImageButtonDraw(const std::string& normal,
        const std::string& hovered,
        const std::string& pushed,
        const glm::vec2& size)
    :normal(Game::getResources()->findSprite(normal)),
     hovered(Game::getResources()->findSprite(hovered)),
     pushed(Game::getResources()->findSprite(pushed)){
    setSize(size);
}

void ImageButtonDraw::setNormal(const std::string& name){
    normal = Game::getResources()->findSprite(name);
}

void ImageButtonDraw::setHovered(const std::string& name){
    hovered = Game::getResources()->findSprite(name);
}

void ImageButtonDraw::setPushed(const std::string& name){
    pushed = Game::getResources()->findSprite(name);
}

void ImageButtonDraw::setSize(const glm::vec2& size){
    scale = size/normal->getSize();
}

void ImageButtonDraw::draw(){
    if(parent->pushed){
        pushed->draw(createMatrix(parent->getPosition(),scale), Color::White);
    }else if(parent->isHovered()){
        hovered->draw(createMatrix(parent->getPosition(),scale), Color::White);
    }else{
        normal->draw(createMatrix(parent->getPosition(),scale), Color::White);
    }
    /*Game::getPrimitiveDrawer()->drawRectangleBorder(createMatrix(parent->getPosition(),
            vec2(scale.x*normal->getHW(),scale.y*normal->getHH())),Color::White);*/
}

void AbstractButtonDraw::drawText() {
        vec3 pos = parent->pushed ?
                parent->getPosition() + vec3(parent->text.getScreenHHeight() * 0.25, -parent->text.getScreenHHeight() * 0.25, 0) :
                parent->getPosition();
        parent->text.draw(pos, parent->getActualLAF().font.color);
}

class Button::NullDrawer : public AbstractButtonDraw {
public:
};

class Button::ButtonDrawSimple : public AbstractButtonDraw {
public:

    virtual void draw() override {
        const PrimitiveDrawer *brush = Game::getPrimitiveDrawer();
        const ButtonStateLAF &actualLaf = parent->getActualLAF();

        const ButtonLAF &laf = Game::getLookAndFeel().buttons;
        brush->drawBorderedFilledRectangle(
                parent->getPosition(),
                parent->getSize(),
                laf.borderThickness,
                actualLaf.backColor,
                actualLaf.borderColor);
        drawText();
    }
};

class Button::ButtonDrawNineSliceSeperate : public AbstractButtonDraw {
public:
    ButtonDrawNineSliceSeperate(std::string normal, std::string pushed, std::string hovered,vec2& size){
        if(normal.size() > 0)
            normalSprite.reset(new NineSliceSprite(Game::getResources()->findSpriteInfo(normal), size.x, size.y));
        if(pushed.size() > 0)
            pushedSprite.reset(new NineSliceSprite(Game::getResources()->findSpriteInfo(pushed), size.x, size.y));
        if(hovered.size() > 0)
            hoveredSprite.reset(new NineSliceSprite(Game::getResources()->findSpriteInfo(hovered), size.x, size.y));
    }

    void setSize(const glm::vec2& _size) override {
        if (normalSprite) {
            normalSprite->setSize(_size/ Game::getScreen()->getDensity());
        }
        if (hoveredSprite) {
            hoveredSprite->setSize(_size/ Game::getScreen()->getDensity());
        }
        if (pushedSprite) {
            pushedSprite->setSize(_size/ Game::getScreen()->getDensity());
        }
    }

    void draw() override{
        NineSliceSprite *sprite =
                parent->pushed ? pushedSprite.get() :
                parent->isHovered() ? hoveredSprite.get() :
                        normalSprite.get();
        if (sprite)
            sprite->draw(parent->getPosition());

        drawText();
    }

private:
    unique_ptr<NineSliceSprite> normalSprite, pushedSprite, hoveredSprite;
};


class Button::ButtonDrawNineSlileAnimated : public AbstractButtonDraw {
public:
    ButtonDrawNineSlileAnimated(std::string image, std::string normal, std::string pushed, std::string hovered, vec2& size)
            : image(Game::getResources()->findSpriteInfo(image), size.x, size.y),
              normal(normal),
              pushed(pushed),
              hovered(hovered) {
    }

    void setSize(const glm::vec2& _size) override{
        image.setSize(_size / Game::getScreen()->getDensity());
    }

    void draw() override{
        string frameName = parent->pushed ? pushed :
                parent->isHovered() ? hovered :
                        normal;
        image.draw(parent->getPosition(), frameName);
        drawText();
    }

private:
    NineSliceSprite image;
    string normal, pushed, hovered;
};

Button::Button(const std::string& _text,
        const glm::vec3& pos,
        const glm::vec2& size,
        const std::string& name,
        std::unique_ptr<AbstractButtonDraw>&& _drawer) :

        text(getActualLAF().font,_text),
        drawer(std::move(_drawer))
{
    UIElement::setName(name.size()>0?name:_text);
    UIElement::setPosition(pos);

    const ButtonLAF &laf = Game::getLookAndFeel().buttons;

    vec2 screenSize(SpriteText::toScreenSize(vec2(
            text.getWorldHW() + laf.padding.x + laf.padding.z,
            text.getWorldHH() + laf.padding.y + laf.padding.w)
    ));
    if(size.x>0) screenSize.x=size.x;
    if(size.y>0) screenSize.y=size.y;
    UIElement::setSize(screenSize);

    if(drawer==nullptr){
        if (laf.animatedImage.size() > 0) {
            drawer.reset(new ButtonDrawNineSlileAnimated(laf.animatedImage,
                    laf.normal.imageName,
                    laf.pushed.imageName,
                    laf.hovered.imageName,screenSize));
        } else if (laf.normal.imageName.size() > 0) {
            drawer.reset(new ButtonDrawNineSliceSeperate(
                    laf.normal.imageName,
                    laf.pushed.imageName,
                    laf.hovered.imageName, screenSize));
        } else {
            drawer.reset(new ButtonDrawSimple());
        }
    }
    drawer->setParent(this);
}

Button::Button(SlotType *iter) :
        Button(iter->choiceValue.button->text,
                vec3(iter->x, iter->y, iter->z),
                vec2(iter->hw, iter->hh),
                iter->name, nullptr){

    scriptAction = iter->choiceValue.button->script;
}


const ButtonStateLAF &Button::getActualLAF() const {
    return pushed ? Game::getLookAndFeel().buttons.pushed :
            isHovered() ? Game::getLookAndFeel().buttons.hovered :
                    Game::getLookAndFeel().buttons.normal;
}

void Button::setPosition(const glm::vec3 &pos) {
    UIElement::setPosition(pos);
}

void Button::setValue(const std::string &_text) {
    text.setText(_text);
}

void Button::executePushOnAnimation() {
    pushed = true;
}

void Button::executePushOffAnimation() {
    pushed = false;
}

void Button::setSize(const glm::vec2& size) {
    UIElement::setSize(size);
    drawer->setSize(size);
}

void Button::executeAction() {
    Game::getUserInterface()->fireEvent(getName(), text.getText());
    Game::getScriptSystem()->doString(scriptAction);
}

std::string Button::getValue() const {
    return text.getText();
}

void Button::draw() {
    drawer->draw();
}
