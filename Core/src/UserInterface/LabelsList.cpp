#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Render/Sprites/NineSliceSprite.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/UserInterface/LabelsList.h>
#include <zlib.h>

using namespace game;
using namespace std;
using namespace glm;

const string LabelsList::TYPE_NAME = "LabelsList";

class game::ListElement :public UIElement {
public:
    ListElement(const std::string& name, LabelsList* parent, const vec3& pos = vec3(0,0,0), const vec2& size=vec2(-1,-1)):
        selected(false),
        text(Game::getLookAndFeel().font,name),
        parent(parent){

        setPosition(pos);
        setSize(vec2(size.x>0?size.x:parent->getHW()+parent->laf.padding.x,
                size.y>0?size.y:text.getScreenHHeight()+parent->laf.padding.y));
    }

    virtual void executeAction(){
        if(text.getText().size()>0) {
            parent->refreshCurrentSelection(this);
            Game::getUserInterface()->fireEvent(parent->getName(), text.getText());
        }
    }

    virtual bool executeScroll(bool up){
        return parent->executeScroll(up);
    }

    virtual void draw(){
        if(selected){
            Game::getPrimitiveDrawer()->drawFilledRectangle(
                    createMatrix(getPosition(),vec2(getHW() - parent->laf.borderWidth.x, getHH())),
                    parent->laf.selectionColor);
        }

        vec3 pos = getPosition();
        pos.x-=(getHW()-text.getScreenHWidth()-parent->laf.padding.x);
        text.draw(pos,selected?parent->laf.selectedTextColor:parent->laf.normalTextColor);
    }

    virtual void setValue(const string& value){
        text.setText(value);
    }

    void setSelection(bool val){
        selected = val;
    }

    virtual std::string getValue() const{
        return text.getText();
    }

private:

    bool selected;
    SpriteText text;
    LabelsList* parent;
};

class game::ScrollBar : public UIElement{
public:
    ScrollBar(LabelsList* parent):
        parent(parent),
        laf(Game::getLookAndFeel().labelsList.scroller),
        normal(Game::getResources()->findSpriteInfo(laf.image),0.5f*laf.worldWidth,0.5f*laf.worldHeight),
        hovered(Game::getResources()->findSpriteInfo(laf.hoveredImage),0.5f*laf.worldWidth,0.5f*laf.worldHeight)
    {

        setSize(vec2(0.5f*laf.worldWidth*Game::getScreen()->getWidthDensity(),
                0.5f*laf.worldHeight*Game::getScreen()->getHeightDensityNoAspect()));
        updatePosition();
    }

    void updatePosition(){
        float shw = 0.5f*laf.worldWidth*Game::getScreen()->getWidthDensity();
        float shh = 0.5f*laf.worldHeight*Game::getScreen()->getHeightDensityNoAspect();

        vec3 p = parent->getPosition();
        p.x+=parent->getHW()-shw-laf.borderWidth.x;
        p.z+=0.2;
        p.y+=parent->getHH()-shh-laf.borderWidth.x;
        setPosition(p);
    }

    void draw() override{
        if(isHovered()){
            hovered.draw(getPosition());
        }else{
            normal.draw(getPosition());
        }
    }

    bool executeScroll(bool up) override{
        return parent->executeScroll(up);
    }

    void setVisible(bool flag) override{
        bool need2Show = static_cast<int >(parent->values.size())-static_cast<int >(parent->elements.size())>0;
        UIElement::setVisible(need2Show && flag);
    }

    void processDraggingAction(const glm::vec2& delta) override{
        float minY = parent->getPosition().y-parent->getHH()+getHH()+laf.borderWidth.y;
        float maxY = parent->getPosition().y+parent->getHH()-getHH()-laf.borderWidth.y;

        if(getPosition().y+delta.y>minY && getPosition().y+delta.y<maxY) {
            UIElement::move(vec3(0, delta.y, 0));
            parent->updateFirstIndex();
        }
    }
private:
    LabelsList* parent;
    const ScrollerLAF& laf;
    NineSliceSprite normal, hovered;
};

LabelsList::LabelsList(const glm::vec3& pos, const glm::vec2& size, const std::string& name):
    laf(Game::getLookAndFeel().labelsList),
    firstIndex(0),
    selected(nullptr){
    {
        ElementCollection::setPosition(pos);
        ElementCollection::setSize(size);
        setName(name);
    }{
        ListElement tst("!pTy", this);
        averageHeight = tst.getHH();
    }{
        scrollerHW = 0.5f*laf.scroller.worldWidth*Game::getScreen()->getWidthDensity();
        scrollerHH = getHH()-laf.scroller.borderWidth.y;
    }
    createElements();
    {
        scroll = new ScrollBar(this);
        ElementCollection::add(scroll);
        updateScrollBarVisibility();
    }
}

LabelsList::LabelsList(SlotType* slot):
        laf(Game::getLookAndFeel().labelsList),
        firstIndex(0),
        selected(nullptr){
    {
        ElementCollection::setPosition(vec3(slot->x,slot->y,slot->z));
        ElementCollection::setSize(vec2(slot->hw, slot->hh));
        setName(slot->name);
    }{
        ListElement tst("!pTy", this);
        averageHeight = tst.getHH();
    }{
        LabelsListSlot* lls = slot->choiceValue.labelsList;
        for(TextSlot* iter:lls->elem){
            values.push_back(iter->text);
        }
    }{
        scrollerHW = 0.5f*laf.scroller.worldWidth*Game::getScreen()->getWidthDensity()/*-laf.scroller.borderWidth.x*/;
        scrollerHH = getHH()-laf.scroller.borderWidth.y;
    }
    createElements();
    {
        scroll = new ScrollBar(this);
        ElementCollection::add(scroll);
        updateScrollBarVisibility();
    }
}

void LabelsList::updateScrollBarVisibility(){
    scroll->setVisible(isVisible());
}

void LabelsList::refreshCurrentSelection(ListElement* sel){
    for(auto iter : elements){
        iter->setSelection(false);
    }

    if(sel!=nullptr) {
        sel->setSelection(true);
        selectedValue = sel->getValue();
    }else{
        selectedValue="";
    }
    selected = sel;
}

std::string LabelsList::getValue(int elemNum){
    return firstIndex+elemNum<values.size()?values[firstIndex+elemNum]:"";
}

void LabelsList::createElements(){
    UserInterface* ui = Game::getUserInterface();
    int elemsNum = static_cast<int>(getHH()/averageHeight);
    vec3 curPos = getPosition();
    curPos.x-=scrollerHW;
    curPos.y+=getHH()-averageHeight-laf.padding.y*Game::getScreen()->getHeightDensityNoAspect();
    curPos.z+=0.1;
    for(int i=0; i<elemsNum;++i){
        ListElement* el = new ListElement(getValue(i),this, curPos, vec2(getHW()-scrollerHW,averageHeight));
        elements.push_back(el);
        ElementCollection::add(el);
        if(isManagedByUI()) {
            el->add2UI(ui);
        }
         curPos.y-=averageHeight*2;
    }
}

void LabelsList::updateFirstIndex(){
    long intervalNum = values.size() - elements.size()+1;
    assert(intervalNum>0);

    float _2_ny = 1.0f-(scroll->getPosition().y - getPosition().y)/((getHH()-scroll->getHH()-laf.scroller.borderWidth.y));
    unsigned newfirstIndex = static_cast<unsigned int>(_2_ny*intervalNum*0.5f);
    if(newfirstIndex!=firstIndex){
        firstIndex=newfirstIndex;
        refreshElementsValues();
    }
}


void LabelsList::deleteElements(){
    if(isManagedByUI()) {
        for(ListElement* el : elements){
            Game::getUserInterface()->remove(el);
        }
    }

    for(ListElement* el : elements){
        ElementCollection::remove(el);
        delete el;
    }
    elements.clear();
}

void LabelsList::add(const std::string& value){
    values.push_back(value);
    refreshElementsValues();
    updateScrollBarVisibility();
}

void LabelsList::add(const std::vector<std::string>& batch){
    values.insert(values.end(),batch.begin(),batch.end());
    refreshElementsValues();
    updateScrollBarVisibility();
}

void LabelsList::add(const std::set<std::string>& batch){
    values.insert(values.end(),batch.begin(),batch.end());
    refreshElementsValues();
    updateScrollBarVisibility();
}

void LabelsList::clear(){
    values.clear();
    for(unsigned i=0;i<elements.size();++i){
        elements[i]->setValue("");
        elements[i]->setSelection(false);
    }
    updateScrollBarVisibility();
}

void LabelsList::refreshElementsValues(){
    for(unsigned i=0;i<elements.size();++i){
        elements[i]->setValue(getValue(i));
    }
    if(selectedValue.size()>0){
        for(unsigned i=0;i<elements.size();++i){
            elements[i]->setSelection(elements[i]->getValue()==selectedValue);
        }
    }
}

void LabelsList::setSize(const glm::vec2& size){
    deleteElements();
    ElementCollection::setSize(size);
    createElements();
    scroll->updatePosition();
}

bool LabelsList::executeScroll(bool up){
    scroll->processDraggingAction(vec2(0,0.05*(up?1:-1)));
    return true;
}

void LabelsList::draw(){
    Game::getPrimitiveDrawer()->drawBorderedFilledRectangle(
            getPosition(), getSize(), laf.borderWidth.x, laf.backgroundColor, laf.borderColor);
    if(scroll->isVisible()){
        Game::getPrimitiveDrawer()->drawBorderedFilledRectangle(
                getPosition() + vec3(getHW() - scrollerHW, 0, 0),
                vec2(scrollerHW, scrollerHH),
                laf.scroller.borderWidth.x,
                laf.scroller.backColor,
                laf.scroller.borderColor);
    }
}

std::string LabelsList::getValue() const {
    return selectedValue;
}
