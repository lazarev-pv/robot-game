#ifndef __LINE_CONTAINER_HEADER_
#define __LINE_CONTAINER_HEADER_


#include <ChibiEngine/UserInterface/ElementCollection.h>
#include <vector>

class SlotType;
class LineContainerSlot;
namespace game{

    class LineContainer : public ElementCollection{
    public:
        LineContainer(SlotType* iter);

        inline void setSize(const glm::vec2& size) override;
        void draw() override;

        inline const std::string& getTypeName() override;

    private:
        const static std::string TYPE_NAME;
        void pack();
        std::vector<float> calcHeights();
        void addElementsAndWeights(LineContainerSlot* slot);

        float yIntent, xIntent;
        Color backColor;
        std::vector<std::vector<float>> weights;
    };

    inline const std::string& LineContainer::getTypeName(){
        return TYPE_NAME;
    }

    inline void LineContainer::setSize(const glm::vec2& size){
        ElementCollection::setSize(size);
        pack();
    }
}

#endif