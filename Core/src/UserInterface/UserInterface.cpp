#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Common/ContainerUtils.h>

#include <ChibiEngine/UserInterface/Button.h>
#include <ChibiEngine/UserInterface/Image.h>
#include <ChibiEngine/UserInterface/TextLabel.h>
#include <ChibiEngine/UserInterface/InputField.h>
#include <ChibiEngine/UserInterface/TableContainer.h>
#include <ChibiEngine/UserInterface/DropDownListBox.h>
#include <ChibiEngine/UserInterface/Checkbox.h>
#include <ChibiEngine/UserInterface/Healthbar.h>
#include <ChibiEngine/UserInterface/Dialog.h>
#include <ChibiEngine/UserInterface/Slider.h>
#include <ChibiEngine/UserInterface/RGBSlider.h>
#include <ChibiEngine/UserInterface/ImagePicker.h>
#include <ChibiEngine/UserInterface/Menu.h>
#include <ChibiEngine/UserInterface/Radio.h>
#include <ChibiEngine/UserInterface/LabelsList.h>
#include <ChibiEngine/UserInterface/LineContainer.h>
#include <ChibiEngine/UserInterface/ContainerSelector.h>

#include <ChibiEngine/Clock/ClockSystem.h>
#include <ChibiEngine/Clock/EventSystem.h>

#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Script/Script.h>

#include <sstream>

#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/Common/ParserUtils.h>

using namespace game;
using namespace glm;
using namespace std;
using namespace rapidxml;

const static string LOGGER_NAME = "UserInterface";

static const Clock::TimeDelta cursorBlinkPeriod = 200;
static const Clock::TimeDelta KEY_REPEAT_DELAY_START = 200;
static const Clock::TimeDelta KEY_REPEAT_INTERVAL = 100;


void UserInterface::toogleCursor() {
    cursorVisible = !cursorVisible;
    if (focusedElement != nullptr) {
        focusedElement->executeViewCursor(cursorVisible);
    }
}

void UserInterface::repeatKey() {
    assert(keyCode != 0);

    if (focusedElement != nullptr) {
        focusedElement->onKeyPressed(keyCode, special);
    }
}

/** Сфокусированный элемент поглощает все нажатия */
bool UserInterface::onKeyPressed(KeyCode keyCode, InputEvent::State state, bool special) {
    if (state == InputEvent::PRESSED) {
        if (focusedElement != nullptr) {
            focusedElement->onKeyPressed(keyCode, special);

            this->keyCode = keyCode;
            this->special = special;
            if (keyRepeatEvent) {
                keyRepeatEvent = Game::getEventSystem()->scheduleInfiteRepeat(clock, bind(&UserInterface::repeatKey, this), clock->getTime() + KEY_REPEAT_DELAY_START, KEY_REPEAT_INTERVAL);
            }
        }

        return focusedElement != nullptr;
    } else if (state == InputEvent::RELEASED) {
        Game::getEventSystem()->deleteEvent(clock, keyRepeatEvent);
        keyRepeatEvent = NullEventHandle;
    }

    return false;
}

UIElement *UserInterface::createElement(SlotType *iter, UIElement* parent) {
    UIElement *el = nullptr;
    //  ----------- КНОПКА
    if (iter->choiceValue.button != nullptr) {
        Button *b = new Button(iter);
        el = b;
        buttonIndex[b->getName()] = b;
    } else if (iter->choiceValue.image != nullptr) {
        el = new Image(iter);
    } else if (iter->choiceValue.text != nullptr) {
        el = new TextLabel(iter);
    } else if (iter->choiceValue.tableContainer != nullptr) {
        TableContainer *tabCont = new TableContainer(iter);
        for (auto subIter : iter->choiceValue.tableContainer->slot) {
            tabCont->add(createElement(subIter, tabCont));
        }
        tabCont->pack();
        el = tabCont;
        tableIndex[tabCont->getName()] = tabCont;
    } else if (iter->choiceValue.input != nullptr) {
        InputField *ipf = new InputField(iter);
        el = ipf;
        inputIndex[ipf->getName()] = ipf;
    } else if (iter->choiceValue.ddlb != nullptr) {
        DropDownListBox *lb = new DropDownListBox(iter);
        ddlbIndex[lb->getName()] = lb;
        el = lb;
    } else if (iter->choiceValue.context != nullptr) {
        ButtonContextMenu *b = new ButtonContextMenu(iter);
        el = b;
        if (b->getName().size() > 0) {
            if (contextMenuIndex.find(el->getName()) != contextMenuIndex.end()) {
                logger.warn("Context Menu with name %s already exists!", el->getName().c_str());
            }
            contextMenuIndex[el->getName()] = b;
        }
    } else if (iter->choiceValue.dialog != nullptr) {
        Dialog *d = new Dialog(iter);
        for (auto subIter : iter->choiceValue.dialog->slot) {
            d->add(createElement(subIter));
        }
        el = d;
    } else if (iter->choiceValue.imagePicker != nullptr) {
        ImagePicker *ip = new ImagePicker(iter);
        imagePickerIndex[ip->getName()] = ip;
        el = ip;
    } else if (iter->choiceValue.rgb != nullptr) {
        RGBSlider *s = new RGBSlider(iter);
        el = s;
        rgbSliderIndex[s->getName()] = s;
    } else if (iter->choiceValue.healthbar != nullptr) {
        el = new Healthbar(iter);
    } else if (iter->choiceValue.slider != nullptr) {
        Slider *s = new Slider(iter);
        el = s;
        sliderIndex[s->getName()] = s;
    } else if (iter->choiceValue.checkbox != nullptr) {
        Checkbox *lb = new Checkbox(iter);
        checkboxIndex[lb->getName()] = lb;
        el = lb;
    } else if (iter->choiceValue.radio != nullptr) {
        RadioButton *r = new RadioButton(iter);
        checkboxIndex[r->getName()] = r;
        el = r;
    } else if (iter->choiceValue.labelsList!=nullptr){
        LabelsList* ll = new LabelsList(iter);
        el = ll;
    } else if (iter->choiceValue.lineContainer!=nullptr){
        el = new LineContainer(iter);
    } else if(iter->choiceValue.containerSelector!=nullptr){
        ContainerSelector* cs;
        if(parent==nullptr) {
            cs = new ContainerSelector(iter,nullptr);
        }else{
            TableContainer *contParent = dynamic_cast<TableContainer *>(parent);
            if(contParent==nullptr){
                logger.error("ContainerSelector [%s] parent can be only TableContainer", iter->name.c_str());
                return nullptr;
            }
            cs = new ContainerSelector(iter,contParent);
        }
        containerSelectorIndex[cs->getName()]=cs;
        el=cs;
    }

    //common atrs
    if (el != nullptr) {
        el->setVisible(iter->visible);
        el->setEnabled(iter->enabled);
    }

    return el;
}

void UserInterface::add(UIElement *el) {
    if (el == nullptr)
        return;

    if (el->getName().size() > 0) {
        if (elementIndex.find(el->getName()) != elementIndex.end()) {
            logger.warn("element with name %s already exists!", el->getName().c_str());
        }
        logger.trace("Element with name %s added ", el->getName().c_str());
        elementIndex[el->getName()] = el;
    }
    slots.push_back(unique_ptr<UIElement>(el));
    sort(slots.begin(), slots.end(),
            [](const UserInterface::SlotsList::value_type& el1, const UserInterface::SlotsList::value_type& el2) {
        return el1->getPosition().z < el2->getPosition().z;
    });
}

void UserInterface::remove(UIElement *el) {
    slots.erase(std::remove_if(slots.begin(), slots.end(), [el](const unique_ptr<UIElement>& tel){
        return tel.get()==el;
    }), slots.end());
    lastHovered = nullptr;
    mouseDownElement = nullptr;
    hookedElement = nullptr;
    focusedElement = nullptr;
}

void UserInterface::fireEvent(const std::string &source, const std::string &value) {
    if (source.size() == 0)
        return;

    logger.debug("Fired event: source [%s], value [%s] ", source.c_str(), value.c_str());

    CallbacksIter it = callbacks.find(source);
    if (it == callbacks.cend()) {
        return;
    }

    for (auto iter = it->second.cbegin(); iter != it->second.cend(); ++iter) {
        (*iter)(value);
    }
}

void UserInterface::addEventListener(std::string source, const std::function<void(const std::string &)> &callback) {
    CallbacksIter it = callbacks.find(source);
    if (it == callbacks.cend()) {
        vector<std::function<void(const std::string &)>> list;
        list.push_back(callback);
        callbacks[source] = list;
    } else {
        it->second.push_back(callback);
    }
}

const PrimitiveDrawer *UserInterface::getPrimitiveDrawer() {
    return &primitiveDrawer;
}

UIElement *UserInterface::findElement(const std::string &name) const {
    UIElement *el = get(name, elementIndex);
    if (el == nullptr)
        logger.warn("Element [%s] not found ", name.c_str());
    return el;
}

RGBSlider *UserInterface::findRGBSlider(const std::string &name) const {
    RGBSlider *el = get(name, rgbSliderIndex);
    if (el == nullptr)
        logger.warn("RGBSlider [%s] not found ", name.c_str());
    return el;
}

Slider *UserInterface::findSlider(const std::string &name) const {
    Slider *el = get(name, sliderIndex);
    if (el == nullptr)
        logger.warn("Slider [%s] not found ", name.c_str());
    return el;
}

TableContainer *UserInterface::findTable(const std::string &name) const {
    TableContainer *el = get(name, tableIndex);
    if (el == nullptr)
        logger.warn("Table [%s] not found ", name.c_str());
    return el;
}

ImagePicker *UserInterface::findImagePicker(const std::string &name) const {
    ImagePicker *el = get(name, imagePickerIndex);
    if (el == nullptr)
        logger.warn("ImagePicker [%s] not found ", name.c_str());
    return el;
}

ContainerSelector *UserInterface::findContainerSelector(const std::string &name) const {
    ContainerSelector *el = get(name, containerSelectorIndex);
    if (el == nullptr)
        logger.warn("ContainerSelector [%s] not found ", name.c_str());
    return el;
}

ButtonContextMenu *UserInterface::findContextMenu(const std::string &name) const {
    ButtonContextMenu *el = get(name, contextMenuIndex);
    if (el == nullptr)
        logger.warn("Context menu [%s] not found ", name.c_str());
    return el;
}

Checkbox *UserInterface::findCheckbox(const std::string &name) const {
    Checkbox *el = get(name, checkboxIndex);
    if (el == nullptr)
        logger.warn("Context menu [%s] not found ", name.c_str());
    return el;
}

DropDownListBox *UserInterface::findDDLB(const std::string &name) const {
    DropDownListBox *el = get(name, ddlbIndex);
    if (el == nullptr)
        logger.warn("DDLB [%s] not found ", name.c_str());
    return el;
}

InputField *UserInterface::findInputField(const std::string &name) const {
    InputField *el = get(name, inputIndex);
    if (el == nullptr)
        logger.warn("Input field [%s] not found ", name.c_str());
    return el;

}

Button *UserInterface::findButton(const std::string &name) const {
    Button *el = get(name, buttonIndex);
    if (el == nullptr)
        logger.warn("Button [%s] not found ", name.c_str());
    return el;

}

UserInterface::UserInterface(InputSystem *is, EventSystem *es) :
        mouseDownElement(nullptr),
        lastHovered(nullptr),
        hookedElement(nullptr),
        focusedElement(nullptr),

        mouseIsDown(false),
        lastCoords(0, 0),

        logger(Game::getLoggingSystem()->createLogger(LOGGER_NAME)),
        userInterfaceListener(this),
        primitiveDrawer(),
        keyRepeatEvent(NullEventHandle),
        clock(Game::getClockSystem()->createClock()),
        cursorVisible(true),
        keyCode(0),
        special(false) {

    is->addRawListener(&userInterfaceListener);
    es->scheduleInfiteRepeat(clock, bind(&UserInterface::toogleCursor, this), 0, cursorBlinkPeriod);
}

void UserInterface::loadFromFile(const std::string &path) {
    logger.info("Loading UI from file " + path);

    // loading interface from file
    auto root(createRoot<UserInterfaceParserRoot>(path));

    string prefix = FileSystem::getPath(path)+"/";
    if(root.userInterface->scriptsFile.size()>0) {
        Game::getScriptSystem()->loadFile(prefix + root.userInterface->scriptsFile);
    }
    Game::getScreen()->setLookAndFeel(LookAndFeel::create(prefix+root.userInterface->lookAndFeel));

    if (root.userInterface->menu != nullptr) {
        Menu *menu = new Menu(root.userInterface->menu);
        menu->add2UI(this);
    }
    for (auto iter : root.userInterface->slot) {
        UIElement *el = createElement(iter);
        if (el != nullptr)
            el->add2UI(this);
    }
    logSortOrder();
}

void UserInterface::logSortOrder(){
    logger.debug("Sort order:");
    for (auto& iter: slots) {
        if (iter->getName().size() > 0) {
            logger.debug("[%s], name [%s], z [%f], visible [%s]",
                    iter->getTypeName().c_str(),
                    iter->getName().c_str(),
                    iter->getPosition().z,
                    iter->isVisible() ? "true" : "false");
        } else {
            logger.trace("[%s], name [%s], z [%f], visible [%s]",
                    iter->getTypeName().c_str(),
                    iter->getName().c_str(),
                    iter->getPosition().z,
                    iter->isVisible() ? "true" : "false");
        }
    }
}

void UserInterface::draw() {
    glDisable(GL_DEPTH_TEST);
    for (auto& iter : slots) {
        if (iter->isVisible())
            iter->draw();
    }
    if(Game::getScreen()->isEnabledDepthTest()){
        glEnable(GL_DEPTH_TEST);
    }
}

UIElement*UserInterface::findElem(const glm::vec2 &pnt) {
    for (auto it = slots.rbegin(); it != slots.rend(); ++it) {
        if (!(*it)->isVisible())
            continue;

        UIElement *el = (*it)->containPoint(pnt);
        if (el != nullptr && el->isVisible()  && el->isEnabled()) {
            return el;
        }
    }
    return nullptr;
}

bool UserInterface::scroll(bool up){
    UIElement *clikedElem = findElem(userInterfaceListener.getMousePos());
    if(clikedElem!=nullptr){
        return clikedElem->executeScroll(up);
    }
    return false;
}

bool UserInterface::click(const glm::vec2 &pnt, ClickType clickType) {
    logger.trace("Process click: %f %f", pnt.x, pnt.y);
    // trace mouse click
    mouseIsDown = (clickType == CLICK_DOWN);

    UIElement *clikedElem = findElem(pnt);
    //process hooking
    if (mouseIsDown) {
        hookedElement = clikedElem;
        if (focusedElement != nullptr) {
            focusedElement->executeLostFocus();
        }
        focusedElement = clikedElem;
        if (focusedElement != nullptr)
            focusedElement->executeFocused();
    } else {
        hookedElement = nullptr;
    }


    //nothing
    if (clikedElem == nullptr) {
        if (mouseDownElement != nullptr)
            mouseDownElement->executePushOffAnimation();
        mouseDownElement = nullptr;
        return false;
    }

    //instant action
    if (clickType == CLICK_DOWN && clikedElem->isInstantAction()) {
        mouseDownElement = nullptr;
        clikedElem->executeAction();
        return true;
    }

    // deffered action
    if (clickType == CLICK_DOWN) {
        mouseDownElement = clikedElem;
        clikedElem->executePushOnAnimation();
    } else if (clickType == CLICK_UP) {
        if (mouseDownElement != nullptr)
            clikedElem->executePushOffAnimation();

        if (mouseDownElement == clikedElem) {
            mouseDownElement = nullptr;
            if (!clikedElem->isInstantAction()) {
                clikedElem->executeAction();
            }
        }
    }

    return true;
}

void UserInterface::processHover(const glm::vec2 &pnt) {

    UIElement *hoveredElem;
    hoveredElem = findElem(pnt);
    if (hoveredElem == lastHovered)
        return;

    if (lastHovered != nullptr) {
        lastHovered->executeHoverOutAnimation();
        lastHovered = 0;
    }

    if (hoveredElem != nullptr) {
        hoveredElem->executeHoverOnAnimation();
        lastHovered = hoveredElem;
    }
}

bool UserInterface::moveMouseEvent(const glm::vec2 &pnt) {
    if (mouseIsDown && hookedElement != nullptr) {
        hookedElement->processDraggingAction(pnt - lastCoords);
    }
    lastCoords = pnt;
    processHover(pnt);
    return false;
}

