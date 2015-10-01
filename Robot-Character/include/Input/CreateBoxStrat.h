#ifndef __ROBOT_CHARACTER_INPUT_CREATE_BOX_STRAT_
#define __ROBOT_CHARACTER_INPUT_CREATE_BOX_STRAT_

#include <Input/MouseStrategy.h>
#include <ChedLevel.h>

namespace game{


    class CreateBoxStrat : public ChedInputListener::MouseStrategy {
    public:

        static const std::string& NAME;

        CreateBoxStrat(ChedLevel* level);

        void draw(CameraCHandle camera) override;
        void onMove(glm::vec2 pnt) override;
        bool onDown(glm::vec2 pnt, MouseButton code) override;
        bool onUp(glm::vec2 pnt) override;

    private:
        float ex,ey,bx,by;
    };

}

#endif