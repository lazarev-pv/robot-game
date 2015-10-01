#ifndef USERINTERFACE_H
#define	 USERINTERFACE_H

#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Input/Input.h>
#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/UserInterface/UserInterfaceListener.h>
#include <ChibiEngine/Game.h>
#include <functional>
#include <ChibiEngine/Clock/EventSystem.h>

#include <map>
#include "TableContainer.h"
#include <ChibiEngine/Clock/ClockHandles.h>

class SlotType;
namespace game{

	class EventSystem;
	class Event;
	class Resources;
	class ButtonContextMenu;
	class DropDownListBox;
	class Button;
	class InputField;
	class Checkbox;
	class RGBSlider;
	class Slider;
    class TableContainer;
    class ImagePicker;
    class ContainerSelector;
    class UserInterface{
    public:
        enum ClickType { CLICK_DOWN, CLICK_UP };
        using SlotsList = std::vector<std::unique_ptr<UIElement>>;
        
        UserInterface(InputSystem* is, EventSystem* es);
        void loadFromFile(const std::string& path);

        void add(UIElement* el);
        void remove(UIElement* el);

        UIElement* findElement(const std::string& name) const;
        ButtonContextMenu* findContextMenu(const std::string& name) const;
        DropDownListBox* findDDLB(const std::string& name) const;
        Button* findButton(const std::string& name) const;
        InputField* findInputField(const std::string& name) const;
        Checkbox* findCheckbox(const std::string& name) const;
        RGBSlider* findRGBSlider(const std::string& name) const;
        Slider* findSlider(const std::string& name) const;
        TableContainer* findTable(const std::string& name) const;
        ImagePicker* findImagePicker(const std::string& name) const;
        ContainerSelector* findContainerSelector(const std::string& name) const;

        void draw();
        bool click(const glm::vec2 &pnt, ClickType clickType);
        bool scroll(bool up);
        bool moveMouseEvent(const glm::vec2 &pnt);
        bool onKeyPressed(KeyCode keyCode, InputEvent::State state, bool special);

        const PrimitiveDrawer*getPrimitiveDrawer();
        inline const Logger& getLogger() const;
        inline ClockCHandle getClock() const;

        void fireEvent(const std::string& source, const std::string& value);
        void addEventListener(std::string uiElement, const std::function<void(const std::string&)>& callback);

        UIElement* createElement(SlotType* iter, UIElement* parent=nullptr);

        void logSortOrder();
    private:

        void processHover(const glm::vec2 &pnt);
        void toogleCursor();
        void repeatKey();
        UIElement *findElem(const glm::vec2 &pnt);


        SlotsList slots;
        std::map<std::string,UIElement*> elementIndex;
        std::map<std::string, ButtonContextMenu*> contextMenuIndex;
        std::map<std::string, DropDownListBox*> ddlbIndex;
        std::map<std::string, Button*> buttonIndex;
        std::map<std::string, InputField*> inputIndex;
        std::map<std::string, Checkbox*> checkboxIndex;
        std::map<std::string, RGBSlider*> rgbSliderIndex;
        std::map<std::string, Slider*> sliderIndex;
        std::map<std::string, TableContainer*> tableIndex;
        std::map<std::string, ImagePicker*> imagePickerIndex;
        std::map<std::string, ContainerSelector*> containerSelectorIndex;

        typedef std::map<std::string, std::vector<std::function<void(const std::string&)>>>::iterator CallbacksIter;
        std::map<std::string, std::vector<std::function<void(const std::string&)>>> callbacks;

        UIElement* mouseDownElement; // Элемент на котором в прошлый раз мышь, нажала кнопку, сбрасывается, когда была поднята вврех
        UIElement* lastHovered; // Последний элемент, над которым мышь была
        UIElement* hookedElement; //Мышка зацепилась за него, сбрасывается когда кнопка поднята вверх. todo дублирование?
        UIElement* focusedElement; //Мышь выбрала его, сбрасывается когда мышь шелкнула по другому объекту
        
        // mouse moving tracing
        bool mouseIsDown;
        glm::vec2 lastCoords;

        Logger logger;
        UserInterfaceListener userInterfaceListener;
        PrimitiveDrawer primitiveDrawer;
        EventHandle keyRepeatEvent; // Указатель на эвент, чтобы при отпускании клавиши прекратить события удаления
        ClockHandle clock; // У Пользовательского интерфейса должен быть свой таймер

        //Мигание курсора
        bool cursorVisible;
        //Повторение клавиши
        int keyCode;
        bool special;
    };

    inline const Logger& UserInterface::getLogger() const{
        return logger;
    }

    inline ClockCHandle UserInterface::getClock() const{
        return clock;
    }

}
#endif

