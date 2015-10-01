#ifndef __BUTTON_H_
#define __BUTTON_H_

#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>

class SlotType;
namespace game {

    class Button;
    class AbstractButtonDraw {
    public:
        AbstractButtonDraw() : parent(nullptr) {};
        AbstractButtonDraw(const AbstractButtonDraw& ) = delete;
        AbstractButtonDraw& operator=(const AbstractButtonDraw&) = delete;
        AbstractButtonDraw(AbstractButtonDraw&& ) = default;
        AbstractButtonDraw& operator=(AbstractButtonDraw&& ) = default;

        virtual ~AbstractButtonDraw() = default;
        virtual void setSize(const glm::vec2&) {};
    protected:
        friend class Button;

        inline void setParent(Button* button){
            parent = button;
        }
        virtual void draw() {};
        void drawText();
        Button* parent;
    };

    class ImageButtonDraw : public AbstractButtonDraw{
    public:
        ImageButtonDraw(const std::string& normal,
                const std::string& hovered,
                const std::string& pushed,
                const glm::vec2& size);
        void setNormal(const std::string& name);
        void setHovered(const std::string& name);
        void setPushed(const std::string& name);

        void setSize(const glm::vec2& size) override;
    private:
        void draw() override;
        glm::vec2 scale;
        TexturedObject *normal, *hovered, *pushed;
    };

    class ButtonStateLAF;
    class SpriteText;
    class NineSliceSprite;
    class Button : public UIElement {
    public:
        Button( const std::string& text = "",
                const glm::vec3& pos = glm::vec3(0,0,0),
                const glm::vec2& size = glm::vec2(-1,-1),
                const std::string& name = "",
                std::unique_ptr<AbstractButtonDraw>&& drawer = nullptr);
        Button(SlotType *iter);

        inline const std::string &getTypeName() override;
        void setPosition(const glm::vec3 &pos) override;
        void setValue(const std::string &text) override;
        void executePushOnAnimation() override;
        void executePushOffAnimation() override;
        void setSize(const glm::vec2& size) override;
        void draw() override;
        void executeAction() override;
        std::string getValue() const override;

        inline AbstractButtonDraw* getDrawer();
    protected:
        const ButtonStateLAF& getActualLAF() const;

        bool pushed = false;
        SpriteText text;
        std::string scriptAction;
    private:

        const static std::string TYPE_NAME;

        class ButtonDrawSimple;
        class ButtonDrawNineSliceSeperate;
        class ButtonDrawNineSlileAnimated;
        friend class AbstractButtonDraw;
        friend class ImageButtonDraw;
        class NullDrawer;

        std::unique_ptr<AbstractButtonDraw> drawer;
    };

    inline const std::string& Button::getTypeName(){
        return TYPE_NAME;
    }

    inline AbstractButtonDraw* Button::getDrawer(){
        return drawer.get();
    }
}



#endif
