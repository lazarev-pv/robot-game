#ifndef UI_ELEMENT_COLLECTION_H_
#define UI_ELEMENT_COLLECTION_H_

#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>

class SlotType;

namespace game {

    class Button;
    class CollectionCaption;
	class ElementCollection : public UIElement {
	public:
		ElementCollection();

		std::size_t getElementCount();

        virtual UIElement* containPoint(const glm::vec2& pnt) override;
		inline const std::string& getTypeName() override;

		virtual void add(UIElement* elem);
		virtual void remove(UIElement* elem);
		void move(const glm::vec3 &delta) override;
		void setPosition(const glm::vec3& pos) override;
		void add2UI(UserInterface* ui) override;

        void setVisible(bool flag) final override;
        virtual void setSlotsVisible(bool flag);

        void setMinimazed(bool isMin);
        inline void toggleMinimalizing();
        inline bool isMinimazed() const;
    protected:
        void setCaption(const std::string& captText);
        inline CollectionCaption* getCaption() const;

        void setSize2AllSlots(const glm::vec2& size);
        void setHW2AllSlots(float hw);
        void remove_if(const std::function<bool(const UIElement*)>& predicate);
        size_t getSlotsSize();
        UIElement* getSlotElem(size_t index);


	private:
		const static std::string TYPE_NAME;
        bool minimalized = false;
        CollectionCaption* caption = nullptr;
        std::vector<UIElement* > slots;
	};

	inline const std::string& ElementCollection::getTypeName(){
		return TYPE_NAME;
	}

    class CollectionCaption : public ElementCollection{
    public:
        CollectionCaption(const glm::vec3& pos, const std::string& text, ElementCollection* parent);

        UIElement* containPoint(const glm::vec2& pnt) override;
        const std::string& getTypeName() override;
        void executeHoverOnAnimation() override;
        void executeHoverOutAnimation() override;
        void draw() override;
        void processDraggingAction(const glm::vec2& delta) override;

        void setValue(const std::string& val) override;
        std::string getValue() const override;

    private:
        ElementCollection* parent;
        SpriteText text;
    };

    inline bool ElementCollection::isMinimazed() const{
        return minimalized;
    }

    inline void ElementCollection::toggleMinimalizing(){
        setMinimazed(!minimalized);
    }

    inline CollectionCaption* ElementCollection::getCaption() const{
        return caption;
    }

}  // namespace game

#endif
