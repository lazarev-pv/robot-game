
#ifndef __UI_IMAGE_H_
#define __UI_IMAGE_H_

#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/Render/Sprites/DrawableObject.h>

class SlotType;
namespace game {

class Image : public UIElement {
	public:
		Image(const DrawableObject* image, const glm::vec3& pos=glm::vec3(0,0,0), const glm::vec2& size = glm::vec2(-1,-1));
		Image(SlotType* image);

        inline const DrawableObject* getImage() const;
        inline const glm::vec2& getScale() const;
		inline const std::string& getTypeName() override;
        std::string getValue() const override;

		void draw() override;
        void setImage(const DrawableObject* image);
        void setValue(const std::string& text) override;

	private:
        void calcScale();

		const static std::string TYPE_NAME;
		const DrawableObject* image;
        glm::vec2 scale;
	};


    inline const DrawableObject* Image::getImage() const{
        return image;
    }

    inline const glm::vec2& Image::getScale() const{
        return scale;
    }
    inline const std::string& Image::getTypeName(){
        return TYPE_NAME;
    }

}  // namespace game


#endif /* IMAGE_H_ */
