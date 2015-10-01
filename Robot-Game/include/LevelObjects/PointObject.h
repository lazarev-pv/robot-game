#ifndef __POINT_OBJECT_H_
#define __POINT_OBJECT_H_

namespace game{

    class PointObject : public LevelObject{
    public:
        inline PointObject(const glm::vec3& pos,int uid,Layer* layer,const std::string& name);
        virtual glm::vec3 getPosition();
        virtual void draw(CameraCHandle camera, const Color &ambientLight){};
    protected:
        virtual ~PointObject(){};
    private:
        glm::vec3 pos;
    };


    inline PointObject::PointObject(const glm::vec3& pos,int uid,Layer* layer,const std::string& name)
        :LevelObject(uid,layer,0,name),pos(pos){}

    glm::vec3 PointObject::getPosition(){
        return pos;
    }
}

#endif

