#include <ChibiEngine/UserInterface/ElementCollection.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/UserInterface/Button.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Common/RAIIUtils.h>

using namespace game;
using namespace std;
using namespace glm;

const string ElementCollection::TYPE_NAME = "ElementCollection";
static const string CollectionCaption_TYPE_NAME = "CollectionCaption";

ElementCollection::ElementCollection():UIElement(){}


std::size_t ElementCollection::getElementCount(){
	return slots.size();
}

void ElementCollection::setVisible(bool flag){
    if(caption)
        caption->setVisible(flag);
    setSlotsVisible(flag);
}


UIElement* ElementCollection::containPoint(const glm::vec2& ){
	return nullptr;
}

void ElementCollection::add(UIElement* elem){
	if(elem!=nullptr){
       // elem->move(vec3(0,0, getPosition().z));
		slots.push_back(elem);
    }
}

void ElementCollection::remove(UIElement* elem){
	if(elem!=nullptr){
		 slots.erase(std::find(slots.begin(), slots.end(), elem));
	}
}

void ElementCollection::move(const glm::vec3 &delta){
	UIElement::move(delta);
	for(auto iter : slots){
		iter->move(delta);
	}
}

void ElementCollection::setPosition(const glm::vec3& pnt){
	move(pnt - getPosition());
}

void ElementCollection::add2UI(UserInterface* ui){
    UIElement::add2UI(ui);
	for(auto iter: slots){
		iter->add2UI(ui);
	}
}


size_t ElementCollection::getSlotsSize(){
    return caption?slots.size()-1:slots.size();
}

UIElement* ElementCollection::getSlotElem(size_t index){
    return caption?slots[index+1]:slots[index];
}

void ElementCollection::setMinimazed(bool isMin){
    assert(caption);

    minimalized = isMin;
    setSlotsVisible(!isMin);
}

void ElementCollection::setSize2AllSlots(const glm::vec2& size){
    for(auto iter : slots){
        iter->setSize(size);
    }
}

void ElementCollection::setHW2AllSlots(float hw){
    for(auto iter : slots){
        iter->setSize(vec2(hw, iter->getHH()));
    }
}

void ElementCollection::remove_if(const std::function<bool(const UIElement*)>& predicate){
    slots.erase(std::remove_if(slots.begin(), slots.end(), predicate),slots.end());
}

void ElementCollection::setCaption(const std::string& captText){
    // has caption - modify text
    if(caption){
        caption->setValue(captText);
        return;
    }
    // no caption - create text
    caption = new CollectionCaption(getPosition(),captText,this);
    //add(caption);
    slots.insert(slots.begin(), caption);
}

void CollectionCaption::setValue(const std::string& newCapt) {
    text.setText(newCapt);
}

std::string CollectionCaption::getValue() const {
    return text.getText();
}


//////////////////////// CollectionCaption ///////////////////////////

CollectionCaption::CollectionCaption(const glm::vec3& pos, const std::string& _text, ElementCollection* parent)
    :parent(parent),
    text(Game::getLookAndFeel().font,_text){

    setPosition(pos+vec3(0.0f,0.0f,0.1f));
    setSize(vec2(parent->getHW(), Game::getLookAndFeel().caption.hh));

    vec2 butSize(getHH()/Game::getScreen()->getAspectRatio(),getHH());
    Button* min = new Button (
            "",
            vec3(0,0,0),
            butSize,
            "back",
            make_unique<ImageButtonDraw>(
                    "ui.collapse.normal",
                    "ui.collapse.hovered",
                    "ui.collapse.pushed",butSize));

    Button* closeButton = new Button (
            "",
            vec3(0,0,0),
            butSize,
            "back",
            make_unique<ImageButtonDraw>(
                    "ui.close.normal",
                    "ui.close.hovered",
                    "ui.close.pushed",butSize));

    add(closeButton);
    add(min);

    min->setPosition(vec3(pos.x+parent->getHW()-(min->getHW()+closeButton->getHW()*2),pos.y,pos.z+0.2));
    closeButton->setPosition(vec3(pos.x+parent->getHW()-(closeButton->getHW()),pos.y,pos.z+0.2));

    if(parent->getName().size()>0){
        min->setName(parent->getName()+"_min");
        Game::getUserInterface()->addEventListener(min->getName(), [parent](const string&){
            parent->toggleMinimalizing();
        });
    }

    if(parent->getName().size()>0){
        closeButton->setName(parent->getName()+"_close");
        Game::getUserInterface()->addEventListener(closeButton->getName(), [parent](const string&){
            parent->setVisible(false);
        });
    }
}

const std::string& CollectionCaption::getTypeName(){
    return CollectionCaption_TYPE_NAME;
}

UIElement* CollectionCaption::containPoint(const glm::vec2& pnt){
    return UIElement::containPoint(pnt);
}

void CollectionCaption::executeHoverOnAnimation(){
    ElementCollection::executeHoverOnAnimation();
    Game::getInputSystem()->setMouseCursor(Cursor::HAND);
}

void CollectionCaption::executeHoverOutAnimation(){
    ElementCollection::executeHoverOnAnimation();
    Game::getInputSystem()->setMouseCursor(Cursor::USUAL);
}

void CollectionCaption::draw(){
    Game::getPrimitiveDrawer()->drawFilledRectangle(
            createMatrix(getPosition(),getSize()),
            Game::getLookAndFeel().caption.color);
    text.draw(getPosition(),Game::getLookAndFeel().caption.textColor);
}

void CollectionCaption::processDraggingAction(const glm::vec2& delta){
    ElementCollection::processDraggingAction(delta);
    parent->move(vec3(delta, 0));
}

void ElementCollection::setSlotsVisible(bool flag) {
    for(size_t i=caption?1:0;i<slots.size();++i) {
        slots[i]->setVisible(flag);
    }
    UIElement::setVisible(flag);
}
