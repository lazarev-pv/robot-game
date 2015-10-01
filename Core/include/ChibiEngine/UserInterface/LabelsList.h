#ifndef __HEADER_LABELS_LIST_
#define __HEADER_LABELS_LIST_

#include <vector>
#include <set>

class SlotType;
namespace game{

    class ScrollBar;
    class ListElement;
    class LabelsList : public ElementCollection {
    public:
        LabelsList(SlotType* slot);
        LabelsList(const glm::vec3& pos, const glm::vec2& size, const std::string& name);

        void add(const std::string& value);
        void add(const std::vector<std::string>& batch);
        void add(const std::set<std::string>& batch);
        void clear();
        void setSize(const glm::vec2& size) override;
        bool executeScroll(bool up) override;
        inline const std::string& getTypeName() override;
        void draw() override;
        std::string getValue() const override;

    private:
        friend class ListElement;
        friend class ScrollBar;
        void createElements();
        void refreshCurrentSelection(ListElement* sel);
        std::string getValue(int elemNum);
        void refreshElementsValues();
        void deleteElements();
        void updateScrollBarVisibility();
        void updateFirstIndex();

        const static std::string TYPE_NAME;

        const LabelsListLAF& laf;
        unsigned firstIndex;
        ListElement* selected;
        std::string selectedValue;
        ScrollBar* scroll;
        float averageHeight;
        float scrollerHW, scrollerHH;
        std::vector<std::string> values;
        std::vector<ListElement*> elements;
    };


    inline const std::string& LabelsList::getTypeName(){
        return TYPE_NAME;
    }
}

#endif