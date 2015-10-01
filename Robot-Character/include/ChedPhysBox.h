#ifndef __PHYSBOX_H_
#define __PHYSBOX_H_

#include <ChedObject.h>
#include <ChibiEngine/Render/Primitives/BoxArea.h>

class CharBoxType;
class PhysicsType;
namespace game{

	class ChedLevel;
	class ChedBone;
	class ChedPhysBox : public ChedObject{
	public:
		ChedPhysBox(ChedLevel* level, const glm::vec2& pos, float hw, float hh);
		ChedPhysBox(ChedLevel* level, CharBoxType* iter);

		void setParent(ChedLevel* level, ChedBone* bone);
		virtual ~ChedPhysBox();


		void rotate(const glm::vec2 &currPnt, const glm::vec2 &delta) override;
		void scale(const glm::vec2& currPnt, const glm::vec2& delta) override;
		void move(const glm::vec2 currPnt, const glm::vec2 &delta) override;
		void draw(CameraCHandle camera) override;

		float getRotation() const override;
		glm::vec2 getPosition() const override;
		glm::vec2 getBegin() const override;
		const glm::vec2 getDirection() const override;

		CharBoxType* save();

		ChedBone *getParent() const {
			return parent;
		}

		float getDensity() const {
			return density;
		}

		void setDensity(float density) {
			this->density = density;
		}

		float getFriction() const {
			return friction;
		}

		void setFriction(float friction) {
			this->friction = friction;
		}

		bool isSensor() const {
			return sensor;
		}

		void setSensor(bool sensor) {
			this->sensor = sensor;
		}

		const std::string& getName() const {
			return name;
		}

		void setName(const std::string& name) {
			this->name = name;
		}

		void parentChanged();

		void skeletonChanged(ChedLevel* level) override;
	private:
		float friction, density;
		std::string name;
		bool sensor;
		ChedBone* parent = nullptr;

		glm::vec2 pos;
		float rotation;

	};

}

#endif
