#ifndef __HEALTH_BAR_H_
#define __HEALTH_BAR_H_

#include <ChibiEngine/UserInterface/UIElement.h>
#include <glm/glm.hpp>
#include <string>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Render/Sprites/NineSliceSprite.h>

class SlotType;
namespace game{

    class NineSliceSprite;
    class HealthbarLAF;
    class Healthbar : public UIElement{
    public:
        Healthbar(SlotType* iter);
        Healthbar(float min,
                float max,
                float value,
                const Color& color,
                float hw);

        inline void setFloatValue(float val);
        inline float getFloatValue() const;

        void draw() override;
        void setValue(const std::string& value) override;
        std::string getValue() const override;
        inline const std::string& getTypeName() override;

    private:
        const static std::string TYPE_NAME;

        const HealthbarLAF& laf;
        float min, max, value;
        Color color;
        NineSliceSprite back;
    };

    inline const std::string& Healthbar::getTypeName(){
        return TYPE_NAME;
    }

    inline void Healthbar::setFloatValue(float val){
        value=glm::clamp(val,min,max);
    }

    inline float Healthbar::getFloatValue() const{
        return value;
    }

}

#endif