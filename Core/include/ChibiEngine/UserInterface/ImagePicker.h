
#ifndef __HEADER_IMAGE_PICKER_
#define __HEADER_IMAGE_PICKER_

#include <ChibiEngine/UserInterface/ElementCollection.h>
#include <ChibiEngine/UserInterface/Button.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/UserInterface/TextLabel.h>
#include <vector>

class SlotType;
namespace game{

    class Image;
    class LabelsList;
    class Resources;
    class ImagePicker : public ElementCollection{
    public:
        ImagePicker(SlotType* slotType);

        void draw() override;
        void addImages(const std::vector<std::string>& names);
        void clearImages();

        const TexturedObject* getSelected() const;
        glm::vec2 getDrawScale() const;
        inline const std::string& getTypeName() override;

    private:
        const static std::string TYPE_NAME;

        const ImagePickerLAF& laf;
        Image* image;
        LabelsList* values;
    };

    inline const std::string& ImagePicker::getTypeName(){
        return TYPE_NAME;
    }

}




#endif /* IMAGEPICKER_H_ */
