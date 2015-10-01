#ifndef __CORE_ANIMATION_H_
#define __CORE_ANIMATION_H_

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <set>
#include <ChibiEngine/Common/ClassUtils.h>
#include <algorithm>
#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/SkeletonAnimation/InterpolationTransformTimeLine_Types.h>
#include <ChibiEngine/SkeletonAnimation/EventTransformTimeLine_Types.h>

class SkeletonAnimationType;
class SkeletonAnimationBoneType;
namespace game{

	class SkeletonJoint;

	class Animation : public Noncopyable{
	public:
		using BoneId = size_t;
		static const BoneId NO_ID = 10000;

		struct InterpolatedTransition {
			inline InterpolatedTransition(
					const glm::vec2& offset,
					const glm::quat& rotation,
					const glm::vec2& scale,
					BoneId boneId);

			glm::vec2 offset;
			glm::vec2 scale;
			glm::quat rotation;
			BoneId boneId;
		};

		struct Bone {

			Bone(BoneId boneId,float duration);
			Bone(SkeletonAnimationBoneType* res,float duration);

			SkeletonAnimationBoneType* save() const;
			inline bool isEmpty() const;
			InterpolatedTransition getInterpolated(float time) const;

			BoneId boneId;
			float duration;

			RotateTransformTimeLine rotations;
			MoveTransformTimeLine moves;
			ScaleTransformTimeLine scales;
		};

		struct TimeStamp {
			typedef std::vector<InterpolatedTransition>::iterator InterTransIt;
			typedef std::vector<InterpolatedTransition>::const_iterator CInterTransIt;

			TimeStamp() = default;

			InterTransIt find(BoneId boneId);
			CInterTransIt find(BoneId boneId) const;
			void sortTransitions();

			std::vector<InterpolatedTransition> transitions;
		};

		using BoneCollectionElement = std::unique_ptr<Bone>;
		using BoneCollection = std::vector<BoneCollectionElement>;


		explicit Animation(SkeletonAnimationType* res);
		inline Animation(const std::string& name);
		Animation();
		Animation(const Animation& other);
		Animation(Animation&& other) = default;

		SkeletonAnimationType* save() const;
		inline const BoneCollection & getBoneAnimations();
		Bone* requestBone(BoneId boneId);
		void deleteBone(std::set<BoneId>& boneId);
		inline EventTransformTimeLine* getEventTimeLine();
		TimeStamp getFrame(float time) const;

		// Animation properties
		void setDuration(float dur);
		inline float getDuration() const;
		inline const std::string& getName() const;

	private:
		using TimeLineIt = BoneCollection::iterator;
		using CTimeLineIt = BoneCollection::const_iterator;

		TimeLineIt findTimeLine(BoneId boneId);
		void sortTimeLines();

		std::string name;
		float duration;
		BoneCollection boneAnimations;
		EventTransformTimeLine eventTimeLine;
		std::function<void(const std::string&)> eventListeners;
	};

	inline Animation::InterpolatedTransition::InterpolatedTransition(
            const glm::vec2& offset,
            const glm::quat& rotation,
            const glm::vec2& scale,
            BoneId boneId)
		:offset(offset), scale(scale), rotation(rotation), boneId(boneId) {}


	inline float Animation::getDuration() const{
		return duration;
	}

	inline const std::string& Animation::getName() const{
		return name;
	}

    inline const Animation::BoneCollection & Animation::getBoneAnimations(){
        return boneAnimations;
    }

    inline bool Animation::Bone::isEmpty() const{
        return rotations.isEmpty() && moves.isEmpty();
    }

    inline Animation::Animation(const std::string& name):
            name(name),
            duration(0.0f){}

    inline EventTransformTimeLine* Animation::getEventTimeLine(){
        return &eventTimeLine;
    }
}




#endif
