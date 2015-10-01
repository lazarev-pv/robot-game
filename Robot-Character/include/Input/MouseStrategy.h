#ifndef __ROBOT_CHARACTER_INPUT_MOUSE_STRATEGY_
#define __ROBOT_CHARACTER_INPUT_MOUSE_STRATEGY_

#include <Input/ChedInputListener.h>
namespace game{

    class ChedInputListener::MouseStrategy{
    public:

        virtual ~MouseStrategy() = default;
        virtual bool onDown(glm::vec2 pntScreen, MouseButton code);
        virtual bool onUp(glm::vec2 pnt);

        virtual void draw(CameraCHandle camera);
        virtual void onMove(glm::vec2 pntScreen);

        MouseButton getCurrentButton();
        const glm::vec2& getMoveDelta();
        const glm::vec2& getCurrentPos();

        MouseStrategy(ChedLevel* level);
    protected:

        ChedLevel* level;
        glm::vec2 lastPos;
        glm::vec2 clickDownCoords;
        MouseButton currentButton;
        glm::vec2 currentPos;
        glm::vec2 delta;
    };


    class NoneStrat : public ChedInputListener::MouseStrategy{
    public:
        static const std::string& NAME;

        NoneStrat(ChedLevel* level):MouseStrategy(level){};
    };

}


#endif