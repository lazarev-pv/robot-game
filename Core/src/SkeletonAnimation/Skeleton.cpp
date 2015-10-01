#include <ChibiEngine/SkeletonAnimation/Skeleton.h>
#include <ChibiEngine/SkeletonAnimation/SkeletonPicture.h>
#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <glm/gtx/transform.hpp>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Common/RAIIUtils.h>
#include <ChibiEngine/Log/Logger.h>
#include <ChibiEngine/Physics/CategoryReference.h>
#include <ChibiEngine/Common/ClassUtils.h>
#include <stdio.h>

#include "SkeletonDynamicGeometry.cpp"
#include "SkeletonStaticGeometry.cpp"

using namespace game;
using namespace glm;
using namespace std;

static const float MILLISEC_OFFSET = 0.001;

const string Skeleton::ANIMATION_END("end");

void Skeleton::createCharacterBody(b2World *world, const glm::vec2 &startPos, CharPhysType *data, const Category &category){

    b2BodyDef bd;
    bd.position.x = startPos.x;
    bd.position.y = startPos.y;
    bd.type = b2_dynamicBody;
    bd.active = true;
    bd.fixedRotation = true;
    body = world->CreateBody(&bd);

    for(CharBoxType* boxData : data->box){
    	if(boxData->parent.size()>0)
    		continue;

    	b2PolygonShape shape;
    	shape.SetAsBox(boxData->hw,boxData->hh,b2Vec2(boxData->pox_x,boxData->pox_y),0.0);
    	b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = boxData->density;
		fd.friction = boxData->friction;
		fd.isSensor = boxData->sensor;
		//if(!fd.isSensor){
			fd.filter.categoryBits=category.category;
			fd.filter.maskBits=category.mask;
	/*	}else{
            Category cat = Game::getPhysicsSystem()->getCategoryReference()->get(
                    category.name==CategoryReference::PROTAGONIST?
                    CategoryReference::PROTAGONIST_WEAPON_SENSOR:
                    CategoryReference::ENEMY_WEAPON_SENSOR
            );
            fd.filter.categoryBits=cat.category;
            fd.filter.maskBits=cat.mask;
        }*/
		{
			staticGeomParts.emplace_back(boxData);
			fd.userData = staticGeomParts.back().getName();
		}
		body->CreateFixture(&fd);

    }
}

void Skeleton::changeSlotImage(const std::string& name, const std::string& imgName){
	SkeletonSlot* slot = indexedSlots[name];
	if(slot)
	    slot->setActive(imgName);
}

Skeleton::Skeleton(const glm::vec2 &startPos, CharacterType* data, const Category& category){
	map<string, SkeletonJoint*> indexedBones;

	if(data==nullptr){
		return;
	}

	// Загружаем кости и картинки
	for(auto iter : data->skeleton->bones){
		SkeletonJoint* jnt = new SkeletonJoint(iter);

		bones.insert(make_pair<>(iter->id, unique_ptr<SkeletonJoint>(jnt)));
		boneLastId=glm::max(boneLastId,iter->id);
		indexedBones[iter->name]=jnt;
		for(auto slot : iter->slot){
            pics.push_back(make_unique<SkeletonSlot>(slot,jnt));
            if(slot->name.size()>0){
                indexedSlots.insert(make_pair(slot->name,pics.back().get()));
            }
		}
	}

	// Выставляем предков
	for(auto iter : data->skeleton->bones){
		if(iter->parent.size()>0){
			indexedBones[iter->name]->setParent(indexedBones[iter->parent]);
		}
	}
	// Сортируем картинки по zOrder
	sort(pics.begin(),pics.end(),[](const std::unique_ptr<SkeletonSlot>& o1, const std::unique_ptr<SkeletonSlot>& o2){
		return o1.get()->getZOrder()<o2.get()->getZOrder();
	});

	// Загружаем анимации
	if(data->animations!=nullptr){
		for(auto iter: data->animations->animation){
			animations.emplace(std::piecewise_construct,
					std::forward_as_tuple(iter->name),
					std::forward_as_tuple(iter));
		}
	}

	/*refreshAnimation();*/
	for(auto& slot : pics){
		slot->getActive().refreshDrawMatrix();
	}


	//  --- Создаем физическую оболочку
    b2World* world = Game::getPhysicsSystem()->getWorld();
	createCharacterBody(world, startPos, data->physics, category);
	// Сенсоры костей:
	for(CharBoxType* d : data->physics->box){
		if(d->parent.size()>0){
            Category c = CategoryReference::get(
                    // todo похоже на костыль
					category.name==CategoryReference::PROTAGONIST?
                            CategoryReference::PROTAGONIST_WEAPON_SENSOR:
                            CategoryReference::ENEMY_WEAPON_SENSOR
            );
			dynamicGeomParts.emplace_back(world,d,indexedBones[d->parent],c);
		}
	}

    ClockHandle clock = Game::getLevelClock();
	nextFrameEvent = Game::getEventSystem()->scheduleInfiteRepeat(clock, bind(&Skeleton::moveCurrentTime, this),
			clock->getTime() + static_cast<Clock::Time>(MILLISEC_OFFSET * 100.0f),
			static_cast<Clock::TimeDelta>(MILLISEC_OFFSET * 100.0f));

}

std::vector<b2Body*> Skeleton::getBoneSensors(){
	vector<b2Body*> res;
	for(auto& box : dynamicGeomParts){
		res.push_back(box.getBody());
	}
	return res;
}

Skeleton::~Skeleton(){
	Game::getEventSystem()->deleteEvent(Game::getLevelClock(), nextFrameEvent);
	//Game::getPhysicsSystem()->deleteObject(body);
}

void Skeleton::draw(CameraCHandle camera,const Color& ambientLight) const{
	const b2Vec2 p = body->GetPosition();
	mat4 mvp = camera->getMVPMatrix()*translate(vec3(p.x,p.y,0))*scale(vec3(reflected?-1.0f:1.0f,1.0f,1.0f));

	for(auto& pic : pics){
		pic->getActive().draw(mvp,ambientLight);
	}
}

void Skeleton::drawBoxes(CameraCHandle camera) const{
	const b2Vec2 p = body->GetPosition();
	vec3 pos(p.x,p.y,0);
	for(const auto& it : staticGeomParts){
		it.draw(pos,camera);
	}
	for(const auto& it : dynamicGeomParts){
		it.draw(camera);
	}
}

void Skeleton::setCurrentTime(float time){
	for(auto& timeline:timelines){
		timeline.second->setCurrentTime(time);
	}
}

std::vector<std::string> Skeleton::getAnimationNames() const{
	vector<string> res;
	for(auto& it : animations){
		res.push_back(it.first);
	}
	return res;
}

void Skeleton::saveAnimations(std::vector<SkeletonAnimationType*>& xmlData) const{
	for(auto& iter : animations){
		xmlData.push_back(iter.second.save());
	}
}

Skeleton::AnimationTimeLine* Skeleton::addAnimation(const std::string& name, bool cycled, bool removeOnEnd){
	AnimationHandle anim2add = animations.find(name);
	if(anim2add==animations.end()){
		return nullptr;
	}

	removeAnimation(name);
	AnimationTimeLine* tl= new AnimationTimeLine(this,anim2add,cycled,removeOnEnd);
	timelines.insert(make_pair(name, unique_ptr<AnimationTimeLine>(tl)));
	return tl;
}

void Skeleton::removeAnimation(const std::string& name){
	timelines.erase(name);
}

void Skeleton::removeAllAnimations(){
	timelines.clear();
}

AnimationHandle Skeleton::createNewAnimation(const std::string& name){
	animations.insert(make_pair(name, Animation(name)));
	return animations.find(name);
}

void Skeleton::deleteAnimation(const std::string& name){
	removeAnimation(name);
	animations.erase(name);
}

void Skeleton::refreshAnimation(){
    for(auto& b : bones) {
        b.second->prepareForAnimation();
    }

    for(auto& timeline : timelines){
        timeline.second->refreshAnimation();
    }

    for(auto& pic : pics){
        pic->getActive().refreshDrawMatrix();
    }

    for(auto& part: dynamicGeomParts){
        part.applyTransform(reflected, this);
    }
}

void Skeleton::AnimationTimeLine::setCurrentTime(float time){
	currentTime=time;
	moveTime(0);
}

void Skeleton::AnimationTimeLine::refreshAnimation(){
    Animation::TimeStamp frame = animationIter->second.getFrame(currentTime);
    for(auto& b : parent->bones){
        b.second->applyTransition(frame);
    }
}

void Skeleton::AnimationTimeLine::moveTime(float delta){
	currentTime+=delta;

	if(currentTime>animationIter->second.getDuration()){
		if(!cycled){
			currentTime = animationIter->second.getDuration();
			refreshAnimation();
			parent->fireEvent(AnimationEvent{name,Skeleton::ANIMATION_END});
            parent->fireEvent(AnimationTimeEvent{name,currentTime});
			if(removeAtEnd)
				parent->removeAnimation(name);
			return;
		}else{
			currentTime=currentTime-animationIter->second.getDuration();
		}
	}

	refreshAnimation();
	parent->fireEvent(AnimationTimeEvent{name,currentTime});

}

void Skeleton::moveCurrentTime(){
	if(!playing)
		return;

	for(auto& b : bones) {
		b.second->prepareForAnimation();
	}

	for(auto& timeline : timelines){
		timeline.second->moveTime(MILLISEC_OFFSET*playbackMultiplier);
	}

	for(auto& pic : pics){
		pic->getActive().refreshDrawMatrix();
	}

	for(auto& part: dynamicGeomParts){
		part.applyTransform(reflected, this);
	}
}

SkeletonJoint* Skeleton::findJoint(size_t id) const {
    return bones.find(id)->second.get();
}

SkeletonSlot* Skeleton::findSlot(Animation::BoneId id) const{
	for(auto& pic : pics){
		if(pic->getJoint()->getId()==id){
			return pic.get();
		}
	}
	return nullptr;
}

void Skeleton::addJoint(std::unique_ptr<SkeletonJoint>&& joint){
	if(joint->getId()==Animation::NO_ID){
		boneLastId++;
		joint->setId(static_cast<Animation::BoneId >(boneLastId));

	}

	bones.insert(make_pair(joint->getId(),std::move(joint)));
}

void Skeleton::addSlot(std::unique_ptr<SkeletonSlot>&& slot){
	pics.push_back(move(slot));
	indexedSlots.insert(make_pair(pics.back()->getName(),pics.back().get()));
}

void Skeleton::findBoneChildren(std::set<Animation::BoneId>& res, Animation::BoneId rootId){
	for(auto& iter : bones){
		if(iter.second->getParent()==nullptr){
			continue;
		}

		if(iter.second->getParent()->getId()==rootId){
			findBoneChildren(res,iter.second->getId());
		}
	}
	res.insert(rootId);
}

void Skeleton::deleteSlot(SkeletonSlot* slot){
	pics.erase(remove_if(pics.begin(), pics.end(), [slot](std::unique_ptr<SkeletonSlot>& _slot){
		return _slot.get()==slot;
	}), pics.end());

	// delete joint slots from index
	for (auto iter = indexedSlots.begin(); iter != indexedSlots.end();) {
		if (iter->second==slot){
			indexedSlots.erase(iter++);
		}else ++iter;
	}
}

void Skeleton::deleteJointWithChildren(Animation::BoneId id){
	// delete bones
	std::set<Animation::BoneId> children;
	findBoneChildren(children,id);

	// delete joint slots from index
	for (auto iter = indexedSlots.begin(); iter != indexedSlots.end();) {
		if (children.find(iter->second->getJoint()->getId())!=children.end()){
			indexedSlots.erase(iter++);
		}else ++iter;
	}

	// delete joint slots physically
	pics.erase(remove_if(pics.begin(), pics.end(), [&children](std::unique_ptr<SkeletonSlot>& slot){
		return children.find(slot->getJoint()->getId())!=children.end();
	}), pics.end());

	// delete bones
	for(auto& id : children){
		bones.erase(id);
	}

	// delete bones animations
	for(auto& it : animations){
		it.second.deleteBone(children);
	}
}