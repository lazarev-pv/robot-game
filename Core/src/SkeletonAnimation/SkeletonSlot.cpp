#include <ChibiEngine/SkeletonAnimation/SkeletonSlot.h>
#include <ChibiEngine/SkeletonAnimation/SkeletonJoint.h>
#include <ChibiEngine/SkeletonAnimation/SkeletonPicture.h>
#include <ChibiEngine/Utils.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>

using namespace std;
using namespace glm;
using namespace game;

SkeletonSlot::SkeletonSlot(SkeletonJoint* parent,
        const std::string& name,
        int zOrder,
       size_t id)
    :parent(parent),
    name(name),
    zOrder(zOrder),
    active(0),
    id(id){
}

void SkeletonSlot::addPicture(const std::string& name,
        TexturedObjectCHandle pres,
        const glm::vec3& pos,
        const glm::vec2& size,
        float rot){

    string _name = name.size()==0 && presRef.size()==0 ? "default":
                   name.size()==0 ? "slot"+presRef.size():
                   name;

    presRef.emplace_back(this, _name, pres, pos, size, rot);

}

SkeletonSlot::SkeletonSlot(BoneSlotType* slot, SkeletonJoint* bone)
    :SkeletonSlot(bone, slot->name, slot->zOrder, slot->id){

    for(BodyPartType* it : slot->pres) {
        addPicture( it->name,
                    Game::getResources()->findSprite(it->pres),
                    vec3(parse(it->pos),0),
                    parse(it->scale),
                    it->rotation
                );
    }

    setActive(slot->defaultPres);
}

const SkeletonPicture& SkeletonSlot::getActive() const{
    return presRef[active];
}

SkeletonPicture& SkeletonSlot::getActive(){
    return presRef[active];
}

/*
void SkeletonSlot::deleteByParentId(size_t parentId){
    presRef.erase(remove_if(presRef.begin(), presRef.end(),[parentId](const SkeletonPicture& pic){
        return pic.getParent()->getId()==parentId;
    }),presRef.end());

}
*/

SkeletonPicture& SkeletonSlot::getPictureByTextureName(const std::string& name){
    for(size_t i = 0; i<presRef.size();++i){
        if(presRef[i].getTextureObject()->getName()==name){
            return presRef[i];
        }
    }

    Game::getLogger().error("SkeletonSlot Pic by texture name [%s] not found to activate",name.c_str());
    return presRef[0];
}

SkeletonPicture& SkeletonSlot::getPicture(const std::string& name){
    for(size_t i = 0; i<presRef.size();++i){
        if(presRef[i].getName()==name){
            return presRef[i];
        }
    }

    Game::getLogger().error("SkeletonSlot Pic [%s] not found to activate",name.c_str());
    return presRef[0];
}

void SkeletonSlot::setActive(const std::string& name){
    for(size_t i = 0; i<presRef.size();++i){
        if(presRef[i].getName()==name){
            active = i;
            return;
        }
    }

    active = 0;
    Game::getLogger().error("SkeletonSlot Pic [%s] not found to activate",name.c_str());
}
