#ifndef __EDITOR_HEADER__
#define __EDITOR_HEADER__

#include <glm/glm.hpp>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>

namespace game{

	class BoxArea{
	public:
        BoxArea(const glm::vec3& pos, const glm::vec2& size, float borderWidth = INITIAL_BORDER_WIDTH, float rotation = 0.0);

        void setBorderWidth(float width);
        void grab(const glm::vec2& pnt);
        bool contains(const glm::vec2& pnt);
        bool isBorderMoving() const;
        void release();

        void setAllMoveEnabled(bool isEnabled);
        void setNormalColor(const Color& color);
        void drag(const glm::vec2& delta);

        void draw(CameraCHandle cam);
        void draw(CameraCHandle cam, const Color& color);

        glm::vec3 getPosition() const;
        void setPosition(const glm::vec3& pos);
        void setSize(const glm::vec2& size);
        void move(const glm::vec2& delta);
        inline glm::vec2 getSize() const;
        float getHWidth() const;
        float getHHeight() const;

        void setRotation(float angle);
        float getRotation() const;

        void rotate(float rot);
        void rotate(float rot, glm::vec2 pivPnt);

	private:
		constexpr static float INITIAL_HALF_WIDTH = 0.5;
		constexpr static float INITIAL_HALF_HEIGHT = 0.5;
		constexpr static float INITIAL_BORDER_WIDTH = 0.06;

		void resetMoveStates();
		void recalcVerts();
		bool containsPoint(const glm::vec2& pnt, glm::vec2 *box);

		void draw(CameraCHandle cam, const Color& color, const Color& moveColor, const PrimitiveDrawer* drawer);

		glm::vec3 pos;
		glm::vec3 pivotPoint;
        float rotation;
		float hw,hh;
		float borderWidth;
        Color normalColor;
		bool allowAllMove;

        bool leftMoving;
        bool rightMoving;
        bool topMoving;
        bool bottomMoving;

		//boundaries -topleft tr br bl
		glm::vec2 allBox[4];
		glm::vec2 leftBox[4];
		glm::vec2 rightBox[4];
		glm::vec2 topBox[4];
		glm::vec2 bottomBox[4];

	};

    inline glm::vec2 BoxArea::getSize() const{
        return glm::vec2(getHWidth(),getHHeight());
    }
}

#endif
