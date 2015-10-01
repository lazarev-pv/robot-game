#ifndef _ROBOT_JOINT_H_
#define _ROBOT_JOINT_H_

#include <LevelObjects/LevelObject.h>
#include <vector>
#include <Box2D/Box2D.h>

class JointType;
class b2Joint;
namespace game {

    class Layer;
    class Joint : public LevelObject{
    public:
        Joint(int uid, JointType *slot, Layer* layer);

        inline b2Joint *getJoint();
        virtual void draw(CameraCHandle camera, const Color &ambientLight);

        inline virtual glm::vec3 getPosition();
        void requestDelete(bool instant) override;
    private:
        b2Joint *joint;
    };

    inline b2Joint* Joint::getJoint(){
        return joint;
    }


    inline glm::vec3 Joint::getPosition(){
        return glm::vec3(0,0,0);
    }

}


#endif
