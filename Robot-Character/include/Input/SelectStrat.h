#ifndef __ROBOT_CHARACTER_INPUT_SELECT_STRAT_
#define __ROBOT_CHARACTER_INPUT_SELECT_STRAT_

#include <Input/MouseStrategy.h>
namespace game{

    class ChedLevel;
    class SelectStrat : public ChedInputListener::MouseStrategy{
    public:
        static const std::string& NAME;

        SelectStrat(ChedLevel* level);

        bool onDown(glm::vec2 pnt, MouseButton code) override;
        void onMove(glm::vec2 pnt) override;
    };

}

#endif