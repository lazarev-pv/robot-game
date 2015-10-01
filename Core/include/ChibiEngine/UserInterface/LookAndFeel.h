#ifndef __UI_LOOKANDFEEL_H_
#define __UI_LOOKANDFEEL_H_

#include <ChibiEngine/Game.h>
#include <glm/glm.hpp>
#include <string>
#include <ChibiEngine/XMLParser/LafParser.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/Handles.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Render/Color.h>

class ButtonStateLAFType;
class FontLAFType;
class ButtonLAFType;
class DialogCaptureLAFType;
class DialogsLAFType;
class LookAndFeelType;
class MenuLAFType;
class InputFieldLAFType;
class CheckboxLAFType;
class ColorType;
class SliderLAFType;
class RGBSliderLAFType;
class LabelsListLAFType;
class ScrollerLAFType;
class ImagePickerLAFType;
class HealthbarLAFType;
class CaptionLAFType;
namespace game {

    class TexturedObject;
    class NineSlicedSprite;
    class Resources;
    class Font;
    class LookAndFeel;
    class UserInterface;
    class SpriteInfo;
    class ScreenSystem;

    struct FontLAF {
        FontLAF(FontLAFType *data, Resources *res, const LookAndFeel *parent);

        FontCHandle font;
        int size;
        Color color;

        FontLAF() = default;
    };

    struct ButtonStateLAF {
        ButtonStateLAF(ButtonStateLAFType *data, Resources *res, const LookAndFeel *parent);

        FontLAF font;
        Color backColor;
        Color borderColor;
        std::string imageName;

        ButtonStateLAF() = default;
    };

    struct ButtonLAF {
        ButtonLAF(ButtonLAFType *data, Resources *res, const LookAndFeel *parent);

        ButtonStateLAF normal, pushed, hovered;
        float borderThickness;
        glm::vec4 padding;
        std::string animatedImage;

        ButtonLAF() = default;
    };

    struct DialogCaptureLAF {
        DialogCaptureLAF(DialogCaptureLAFType *data, Resources *res, const LookAndFeel *parent);

        float height;
        float controlWidth;
        FontLAF font;
        Color color;
        ButtonLAF controlButtons;

        DialogCaptureLAF() = default;
    };

    struct DialogLAF {
        DialogLAF(DialogsLAFType *data, Resources *res, const LookAndFeel *parent);

        DialogCaptureLAF capture;
        float borderThikness;
        Color backColor;
        Color borderColor;
        std::string image;

        DialogLAF() = default;
    };

    struct InputFieldLAF {
        InputFieldLAF(InputFieldLAFType *data, Resources *res, const LookAndFeel *parent);

        Color backColor;
        Color disBackColor;
        Color selectColor;
        Color selectedBorderColor;
        Color borderColor;
        glm::vec4 padding;
        float borderThickness;
        std::string image;
        std::string normal;
        std::string focused;
        FontLAF font;
        float cursorHW;
        Color cursorColor;

        InputFieldLAF() = default;
    };

    struct MenuLAF {
        MenuLAF(MenuLAFType *data, Resources *res, const LookAndFeel *parent);

        Color backColor;
        Color borderColor;
        Color hoveredColor;
        FontLAF font;
        float borderThickness;
        std::string image;
        std::string hoverImage;
        glm::vec4 padding;

        MenuLAF() = default;
    };

    struct SliderLAF {
        SliderLAF(SliderLAFType *data, Resources *res, const LookAndFeel *parent);

        Color recColor, hoveredColor, markColor;
        float baseHH, hh, markLenX, markLenY;
        Color textColor;
        int defaultPercision;

        SliderLAF() = default;
    };

    struct RGBSliderLAF {
        RGBSliderLAF(RGBSliderLAFType *data);

        float colorHW, hh;

        RGBSliderLAF() = default;
    };

    struct CheckboxLAF {
        CheckboxLAF(CheckboxLAFType *data, Resources *res);

        float hw, hh;
        TexturedObjectHandle normal, checked;
        glm::vec2 normalScale;
        glm::vec2 checkedScale;

        CheckboxLAF() = default;

    };

    struct ScrollerLAF {
        ScrollerLAF(ScrollerLAFType *iter, const LookAndFeel *parent);

        Color backColor;
        Color borderColor;
        std::string image;
        std::string hoveredImage;
        float worldWidth, worldHeight;
        glm::vec2 borderWidth;

        ScrollerLAF() = default;
    };

    struct LabelsListLAF {
        LabelsListLAF(LabelsListLAFType *iter, const LookAndFeel *parent);

        Color normalTextColor;
        Color selectedTextColor;
        Color selectionColor;
        Color backgroundColor;
        Color borderColor;
        glm::vec2 borderWidth;
        ScrollerLAF scroller;
        glm::vec2 padding; //hw, hh

        LabelsListLAF() = default;
    };

    struct ImagePickerLAF{
        ImagePickerLAF(ImagePickerLAFType *iter, const LookAndFeel *parent);

        glm::vec3 labelsOffset;
        glm::vec2 labelsSize;
        Color imageBorderColor;
        glm::vec3 imageOffset;
        glm::vec2 imageSize;
        glm::vec2 baseSize;

        ImagePickerLAF() = default;
    };

    struct HealthbarLAF{
        HealthbarLAF(HealthbarLAFType *iter, const LookAndFeel *parent);
        float hh,defaultPercision;
        TexturedObjectHandle image;
        SpriteInfo::CHandle backImage;
        glm::vec2 padding;

        HealthbarLAF() = default;
    };

    struct CaptionLAF{
        CaptionLAF(CaptionLAFType* iter,const LookAndFeel* parent);
        float hh;
        Color color;
        Color textColor;

        CaptionLAF() = default;
    };

    struct LookAndFeel {

        static LookAndFeel create(const std::string &fileName);

        Color defaultColor;
        glm::vec4 defaultPadding;
        FontLAF font;
        ButtonLAF buttons;
        DialogLAF dialogs;
        MenuLAF menu;
        InputFieldLAF inputField;
        SliderLAF slider;
        CheckboxLAF checkbox;
        RGBSliderLAF rgb;
        LabelsListLAF labelsList;
        ImagePickerLAF imagePicker;
        HealthbarLAF healthbar;
        CaptionLAF caption;

        LookAndFeel(LookAndFeel&& ) = default;
        LookAndFeel& operator=(LookAndFeel &&) = default;
    private:
        LookAndFeel(LookAndFeelType *data, Resources *res);
        LookAndFeel(const LookAndFeel &) = delete;
        LookAndFeel& operator=(const LookAndFeel &) = delete;

        friend class ScreenSystem;
        LookAndFeel() = default;

    };

    Color getColor(ColorType *color, const LookAndFeel *parent);
}


#endif
