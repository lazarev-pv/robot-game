
#ifndef __TABLE_CONTAINER_H_
#define __TABLE_CONTAINER_H_

#include <ChibiEngine/UserInterface/ElementCollection.h>
#include <ChibiEngine/Render/Color.h>

class SlotType;
namespace game {

	class TableContainer : public ElementCollection {
	public:
		TableContainer(SlotType* iter);

		void setSize(const glm::vec2& size) override;
        void draw() override;
		void pack();

		inline const std::string& getTypeName() override;

		virtual UIElement *containPoint(const glm::vec2 &pnt) override;

	private:
        void alignPos();

        const static std::string TYPE_NAME;
        std::vector<float> weights;
        float intent;
        Color backColor;
        bool drawLines;
        std::string align;

	};

	inline const std::string& TableContainer::getTypeName(){
		return TYPE_NAME;
	}

}


#endif
