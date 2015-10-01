#include <ChibiEngine/SkeletonAnimation/Animation.h>

#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <ChibiEngine/Common/RAIIUtils.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Common/ContainerUtils.h>

using namespace std;
using namespace game;
using namespace glm;

Animation::Animation(SkeletonAnimationType* res):
	name(res->name),
	duration(res->duration),
    eventTimeLine(res->event){
	for(auto iter:res->bone){
		boneAnimations.emplace_back(new Bone(iter,duration));
	}
	sortTimeLines();
}
/*

Animation::Animation(const Animation& other)
    :name(other.name),
    duration(other.duration){
    for(const BoneCollectionElement & elem : other.boneAnimations){
        boneAnimations.emplace_back(new Bone(*elem.get()));
    }
}
*/
/*

Animation& Animation::operator=(const Animation& other){
    name = other.name;
    duration = other.duration;
    boneAnimations.clear();
    for(const BoneCollectionElement & elem : other.boneAnimations){
        boneAnimations.emplace_back(new Bone(*elem.get()));
    }
    return *this;
}
*/

Animation::Animation():
        name(),
        duration(){
}

void Animation::sortTimeLines(){
	sort(boneAnimations.begin(), boneAnimations.end(),
            [](const Animation::BoneCollectionElement & t1, const Animation::BoneCollectionElement & t2){
        return t1->boneId<t2->boneId;
    });
}

Animation::TimeLineIt Animation::findTimeLine(BoneId boneId){
	auto first = std::lower_bound(boneAnimations.begin(), boneAnimations.end(), boneId,
                [](const Animation::BoneCollectionElement & t1, const BoneId& t2){
                    return t1->boneId<t2;
                });
	if((first == boneAnimations.end()) || (boneId < (*first)->boneId)){
		return boneAnimations.end();
	}
	return first;
}

SkeletonAnimationType* Animation::save() const{
	SkeletonAnimationType* res = new SkeletonAnimationType();
	res->name=this->name;
	res->duration=duration;

	for(auto& iter : boneAnimations){
        if(iter->isEmpty())
            continue;
		res->bone.push_back(iter->save());
	}
    eventTimeLine.save(res->event);
	return res;
}

Animation::TimeStamp Animation::getFrame(float time) const{
	TimeStamp stamp;
	for(const auto& it : boneAnimations){
		stamp.transitions.push_back(it->getInterpolated(time));
	}
	stamp.sortTransitions();
	return stamp;
}

void Animation::setDuration(float dur){
	this->duration = dur;
	for(auto& tl : boneAnimations){
		tl->duration = dur;
	}
}

void Animation::deleteBone(set<BoneId>& boneIds){
	boneAnimations.erase(remove_if(boneAnimations.begin(),boneAnimations.end(), [&boneIds](BoneCollectionElement& element){
		return boneIds.find(element->boneId)!=boneIds.end();
	}),boneAnimations.end());
}

Animation::Bone* Animation::requestBone(BoneId boneId) {
    auto it = findTimeLine(boneId);
    if(it==boneAnimations.end()){
        Bone* ba = new Bone(boneId,duration);
        boneAnimations.emplace_back(ba);
        sortTimeLines();
        return ba;
    }
    return it->get();
}

/*
void Animation::hardMix(const Animation& other){
	//this->boneAnimations=other.boneAnimations;
	duration = std::max(duration, other.duration);
	for(auto otherTimeLine = other.boneAnimations.begin(); otherTimeLine!=other.boneAnimations.end();++otherTimeLine){
		Animation::TimeLineIt sameTimeLine = findTimeLine(otherTimeLine->boneId);
		if(sameTimeLine==boneAnimations.end()){
			boneAnimations.push_back(*otherTimeLine);
			sortTimeLines();
		}else{
			for(auto otherEvent = otherTimeLine->rotations.begin(); otherEvent!= otherTimeLine->rotations.end(); ++otherEvent){
				Animation::Bone::TransitionIt sameTrans = sameTimeLine->find(otherEvent->time);
				if(sameTrans==sameTimeLine->rotations.end()){
					sameTimeLine->rotations.push_back(*otherEvent);
					sameTimeLine->sortTransitions();
				}else{
					sameTrans->offset=otherEvent->offset;
					sameTrans->rotation=otherEvent->rotation;
				}
			}

		}
	}
}
*/

// -------------------- Bone

Animation::Bone::Bone(SkeletonAnimationBoneType* res, float duration):
	boneId(static_cast<BoneId>(res->id)),
	duration(duration),
    rotations(res->rotate),
    moves(res->offset),
    scales(res->scale){
}

Animation::Bone::Bone(BoneId boneId,float duration):
		boneId(boneId),
		duration(duration){
}

SkeletonAnimationBoneType* Animation::Bone::save() const{
    SkeletonAnimationBoneType* res = new SkeletonAnimationBoneType();
	res->id = static_cast<int>(boneId);
    moves.save(res->offset);
    rotations.save(res->rotate);
    scales.save(res->scale);
	return res;
}

Animation::InterpolatedTransition Animation::Bone::getInterpolated(float time) const{
    return InterpolatedTransition(
            moves.getInterpolated(time,duration),
            rotations.getInterpolated(time,duration),
            scales.getInterpolated(time,duration),
            boneId);
}


// -------------------- Time STAMP
struct CompareInterTransaction{
	bool operator()(const Animation::InterpolatedTransition& t1, Animation::BoneId val){
		return t1.boneId<val;
	}
	bool operator()(const Animation::InterpolatedTransition& t1, const Animation::InterpolatedTransition& t2){
		return t1.boneId<t2.boneId;
	}
};

Animation::TimeStamp::InterTransIt Animation::TimeStamp::find(BoneId boneId){
	InterTransIt first = std::lower_bound(transitions.begin(), transitions.end(), boneId, CompareInterTransaction());
	if((first == transitions.end()) || (boneId < first->boneId)){
		return transitions.end();
	}
	return first;
}

Animation::TimeStamp::CInterTransIt Animation::TimeStamp::find(BoneId boneId) const{
	CInterTransIt first = std::lower_bound(transitions.begin(), transitions.end(), boneId, CompareInterTransaction());
	if((first == transitions.end()) || (boneId < first->boneId)){
		return transitions.end();
	}
	return first;
}

void Animation::TimeStamp::sortTransitions(){
	sort(transitions.begin(),transitions.end(),CompareInterTransaction());
}

