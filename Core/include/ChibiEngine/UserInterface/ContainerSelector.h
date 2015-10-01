#ifndef __CONTAINER_SELECTOR_HEADER_
#define __CONTAINER_SELECTOR_HEADER_


#include <ChibiEngine/UserInterface/UIElement.h>
#include <unordered_map>
#include <string>

class SlotType;
namespace game{

    class TableContainer;
    class ContainerSelector : public UIElement{
    public:
        ContainerSelector(SlotType* iter,TableContainer* parent);

        inline const std::string& getTypeName() override;
        UIElement* containPoint(const glm::vec2& pnt) override;

        void add2UI(UserInterface* ui);
        void setVisible(bool flag) override;
        void setPosition(const glm::vec3& pos) override;
        void setSize(const glm::vec2& size) override;
        void move(const glm::vec3 &delta) override;
        void activate(const std::string elementName);

    private:
        void updateVisibility();

        const static std::string TYPE_NAME;
        TableContainer* parent;
        UIElement* active;
        std::unordered_map<std::string, UIElement*> elRefs;
    };


    inline const std::string& ContainerSelector::getTypeName(){
        return TYPE_NAME;
    }
}

#endif