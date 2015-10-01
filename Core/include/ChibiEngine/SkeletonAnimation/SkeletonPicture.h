#ifndef __CHIBIENGINE_SKELETON_ANIMATION_SKELETON_PICTURE_
#define __CHIBIENGINE_SKELETON_ANIMATION_SKELETON_PICTURE_

#include <ChibiEngine/Handles.h>
#include <ChibiEngine/Common/ClassUtils.h>
#include <ChibiEngine/Render/Color.h>
#include "SkeletonSlot.h"

class BoneType;
class BodyPartType;
namespace game{

	class Skeleton;
    class SkeletonJoint;
    class TexturedObject;
    class Camera;
    class SkeletonPicture : private Noncopyable{
	public:
		SkeletonPicture(SkeletonSlot* parent, BodyPartType* data, const std::string& name);
		SkeletonPicture(SkeletonSlot* parent,
				const std::string& name,
				TexturedObjectCHandle pres,
				const glm::vec3& pos,
				const glm::vec2& size,
				float rot);
		void resetInitMatrix(const glm::vec3& pos, const glm::vec2& size, float rot);

		void draw(const glm::mat4& mvp,const Color& ambientLight) const;
		inline const std::string& getName() const;
		inline const glm::mat4& getDrawMatrix() const;
		inline TexturedObjectCHandle getTextureObject() const;

		void refreshDrawMatrix();
		inline SkeletonSlot* getSlot() const;
	private:
		SkeletonSlot* parent;
		TexturedObjectCHandle pres;
		std::string name;
		glm::mat4 initMatrix;
		glm::mat4 drawMatrix;
	};

	inline const std::string& SkeletonPicture::getName() const{
		return name;
	}

	inline SkeletonSlot* SkeletonPicture::getSlot() const{
		return parent;
	}

	inline const glm::mat4& SkeletonPicture::getDrawMatrix() const{
		return drawMatrix;
	}

	inline TexturedObjectCHandle SkeletonPicture::getTextureObject() const{
		return pres;
	}
}



#endif
