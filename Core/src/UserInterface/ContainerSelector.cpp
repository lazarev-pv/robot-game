#include <ChibiEngine/UserInterface/ContainerSelector.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/UserInterface/TableContainer.h>
#include <ChibiEngine/Log/Logger.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Common/StringUtils.h>
#include <ChibiEngine/Game.h>

using namespace game;
using namespace std;
using namespace glm;

const string ContainerSelector::TYPE_NAME = "ContainerSelector";

ContainerSelector::ContainerSelector(SlotType* iter,TableContainer* parent)
        :parent(parent){
    UIElement::setPosition(vec3(iter->x,iter->y,iter->z));
    UIElement::setSize(vec2(iter->hw,iter->hh));
    UIElement::setName(iter->name);

    UserInterface* ui = Game::getUserInterface();
    for(auto slot : iter->choiceValue.containerSelector->slot){
        if(slot->name.size()==0){
            ui->getLogger().warn("Missed Noname ContainerSlot Element");
            continue;
        }
        elRefs[slot->name]=ui->createElement(slot);
    }

    activate(iter->choiceValue.containerSelector->initial);
}

void ContainerSelector::updateVisibility(){
    for(auto it : elRefs){
        it.second->setVisible(false);
    }
    active->setVisible(UIElement::isVisible());
}

void ContainerSelector::move(const glm::vec3 &delta){
    UIElement::move(delta);
    active->move(delta);
}

void ContainerSelector::add2UI(UserInterface* ui){
    UIElement::add2UI(ui);
    for(auto it : elRefs){
        it.second->add2UI(ui);
    }
}

void ContainerSelector::activate(const std::string elementName){
    active=elRefs[elementName];
    if(active==nullptr) {
        Game::getUserInterface()->getLogger().warn("initial element [%s] not found. Picking first.",
                elementName.c_str());
        active = elRefs.begin()->second;
    }
    UIElement::setPosition(active->getPosition());
    UIElement::setSize(vec2(active->getHW(),active->getHH()));
    updateVisibility();
    parent->pack();

}

UIElement* ContainerSelector::containPoint(const glm::vec2& pnt){
    return active->containPoint(pnt);
}

void ContainerSelector::setVisible(bool flag){
    UIElement::setVisible(flag);
    updateVisibility();
}

void ContainerSelector::setPosition(const glm::vec3& pos){
    UIElement::setPosition(pos);
    active->setPosition(pos);
}

void ContainerSelector::setSize(const glm::vec2& size){
    UIElement::setSize(size);
    active->setSize(size);
}