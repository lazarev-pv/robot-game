#ifndef __ROBOT_CHARACTER_INPUT_CREATE_BONE_STRAT_
#define __ROBOT_CHARACTER_INPUT_CREATE_BONE_STRAT_

#include <Input/MouseStrategy.h>
#include <ChedLevel.h>
#include <Input/BoneTemplate.h>

namespace game{

    class CreateBoneStrat : public ChedInputListener::MouseStrategy {
    public:

        static const std::string& NAME;

        CreateBoneStrat(ChedLevel* level);

        void setParent(ChedBone* parentBone);
        void draw(CameraCHandle camera) override;
        void onMove(glm::vec2 pnt) override;

        bool onUp(glm::vec2 pnt) override;
        bool onDown(glm::vec2 pnt, MouseButton code) override;
    private:
        BoneTemplate bone;
        ChedBone* parentBone = nullptr;
    };

}

#endif