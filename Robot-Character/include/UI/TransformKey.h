#ifndef __ROBOT_CHARACTER_TRANSFORM_KEY_H_
#define __ROBOT_CHARACTER_TRANSFORM_KEY_H_

#include <glm/glm.hpp>
#include <UI/CurveKey.h>
#include <ChibiEngine/Render/Color.h>

namespace game{

    class TransformKey{
    public:
        using Time = int;

        inline TransformKey(Time time=-1);

        inline int getTime() const;
        inline void setTime(Time time);
        inline bool isHighlighted() const;
        inline bool isSelected() const;

        inline void setHighlighted(bool value);
        inline void setSelected(bool value);
        inline bool operator==(const TransformKey& other);
        inline bool operator!=(const TransformKey& other);
        inline bool operator<(const TransformKey& other);

        static void drawImpl(const glm::vec3& pos, const glm::vec2& size, const Color& color, bool highlighted, Time time);
        static glm::vec2 getPositionImpl(const glm::vec3& pos, const glm::vec2& size, Time time);

        void draw(const glm::vec3& pos, const glm::vec2& size, const Color& color);
        glm::vec2 getPosition(const glm::vec3& pos, const glm::vec2& size);

        inline const CurveKey& getCurve();

    protected:
        Time time;
        bool highlighted = false;
        bool selected = false;
        CurveKey curve;
    };


    inline TransformKey::TransformKey(Time time):
            time(time){}

    inline TransformKey::Time TransformKey::getTime() const {
        return time;
    }

    inline void TransformKey::setTime(Time time){
        TransformKey::time = time;
    }

    inline bool TransformKey::operator==(const TransformKey& other){
        return time==other.time;
    }

    inline bool TransformKey::operator!=(const TransformKey& other){
        return time!=other.time;
    }

    inline bool TransformKey::operator<(const TransformKey& other){
        return time<other.time;
    }

    inline bool TransformKey::isHighlighted() const{
        return highlighted;
    }

    inline void TransformKey::setHighlighted(bool value){
        TransformKey::highlighted = value;
    }

    inline const CurveKey& TransformKey::getCurve(){
        return curve;
    }

    inline bool TransformKey::isSelected() const {
        return selected;
    }

    inline void TransformKey::setSelected(bool value) {
        TransformKey::selected = value;
    }

}


#endif

