#ifndef __ROBOT_CHARACTER_UI_CHED_OBJECT_
#define __ROBOT_CHARACTER_UI_CHED_OBJECT_

#include <glm/glm.hpp>
#include <ChibiEngine/Common/EventSupportable.h>
#include <ChibiEngine/Render/Primitives/LinesChain.h>

namespace game{

	class ChedLevel;
	class Camera;
	class ChedBone;
	class ChedObject : public EventSupportableImpl{
	public:
		ChedObject();
		virtual ~ChedObject() = default;

		virtual bool containsPoint(const glm::vec2& pnt);
		virtual void setHighlighted(bool flag);
		virtual void setSelected(bool flag);

		virtual void rotate(const glm::vec2 &currPnt, const glm::vec2 &delta) = 0;
		virtual void scale(const glm::vec2& currPnt, const glm::vec2& delta) = 0;
		virtual void move(const glm::vec2 currPnt, const glm::vec2 &delta) = 0;
		virtual void draw(CameraCHandle camera);

		virtual float getRotation() const = 0;
		virtual glm::vec2 getPosition() const = 0;
		virtual glm::vec2 getBegin() const = 0;
		virtual const glm::vec2 getDirection() const = 0;

		virtual void skeletonChanged(ChedLevel* level) = 0;
	protected:
		void calcBoundVerts(const glm::mat4& animMat);

		bool highligthed;
		bool selected;
		//glm::vec2 pos;
		//float rotation;

		float hh,hw;
		LinesChain normal;
	private:
		glm::vec2 verts[4]; //topleft tr br bl

	};

}



#endif
