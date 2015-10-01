#ifndef __RADIO_BUTTON_H_
#define __RADIO_BUTTON_H_

#include <string>
#include <ChibiEngine/UserInterface/Checkbox.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>

class SlotType;
namespace game{

    class RadioButton : public Checkbox{
    public:
        RadioButton(SlotType* iter);
        RadioButton(const glm::vec3& pos,
                const std::string& name,
                const std::string& group,
                bool value);

        void executeAction() override;
        inline const std::string& getTypeName() override;

    private:
        const static std::string TYPE_NAME;

        std::string groupName;
    };


    inline const std::string& RadioButton::getTypeName(){
        return TYPE_NAME;
    }

}

#endif