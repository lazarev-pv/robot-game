#ifndef __ROBOT_CHARACTER_UI_CHED_SLOT_
#define __ROBOT_CHARACTER_UI_CHED_SLOT_

#include <ChedObject.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include <ChibiEngine/SkeletonAnimation/SkeletonPicture.h>

class BodyPartType;
namespace game{

    class ChedLevel;
    class ChedBone;
    class TexturedObject;
    class SkeletonPicture;
    class SkeletonSlot;
    class ChedSlot : public ChedObject{
    public:
        ChedSlot(ChedLevel* level, SkeletonSlot* pic, BoneSlotType* data, ChedBone* bone);
        ChedSlot(ChedLevel* level, SkeletonSlot* pic, ChedBone* bone);
        void addPicMeta(const std::string& name, const glm::vec3& pos, const glm::vec2& scale, float rot);
        void addPic(const TexturedObject* obj, const std::string& slotName, const glm::vec2& scale);

        void draw(CameraCHandle camera) override;
        void skeletonChanged(ChedLevel* level) override;

        inline SkeletonSlot* getSkeletonSlot();
        float getRotation() const override;
        glm::vec2 getPosition() const override;
        glm::vec2 getBegin() const override;
        const glm::vec2 getDirection() const override;
        void setActiveSlotDefault();
        std::vector<std::string> getSlots() const;

        void move(const glm::vec2 currPnt, const glm::vec2 &delta) override;
        void rotate(const glm::vec2 &currPnt, const glm::vec2 &delta) override;
        void scale(const glm::vec2& currPnt, const glm::vec2& delta) override;

        BoneSlotType* save() const;
    private:

        struct PicMeta {
            glm::vec3 pos;
            glm::vec2 scale;
            float rotation;
        };

        std::unordered_map<std::string, PicMeta> meta;
        SkeletonSlot* slot;
        ChedBone* bone;
        std::string defaultSlot = "";
    };

    inline SkeletonSlot*ChedSlot::getSkeletonSlot(){
        return slot;
    }
}


#endif
