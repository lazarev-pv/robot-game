#ifndef __EDITOR_EVENT_OBJECT_
#define __EDITOR_EVENT_OBJECT_

#include <EditorLevelImpl/BoxAreaObject.h>
#include <EditorLevel.h>
#include <string>
#include <ChibiEngine/Render/Primitives/SpriteText.h>

class EventType;
namespace game{

class EditorGeometryObject;
class Layer;
class Texture;
class EventObject : public BoxAreaObject{
public:

    EventObject(EventType* iter);
    EventObject(const glm::vec2& pos,
            const std::string& onEnter="",
            const std::string& onLeave="",
            const glm::vec2& size=glm::vec2(0.5,0.5),
            bool infinite=false,
            const std::string& name="");
	virtual ~EventObject();

	virtual void draw(CameraCHandle camera);
	virtual void save2World(WorldType* world);
	virtual OperableObject* clone();
	virtual void setSelected(bool isSelected);
    inline virtual void setName(const std::string& name);

    inline void setOnEnter(const std::string& name);
    inline const std::string& getOnEnter() const;
    inline void setOnLeave(const std::string& name);
    inline const std::string& getOnLeave() const;
    inline bool isInfinite() const;
    inline void setInfinite(bool infinite);
private:
    bool infinite;
    int period;
    std::string onEnter, onLeave;
    SpriteText text;
};

    inline void EventObject::setName(const std::string& name){
        BoxAreaObject::setName(name);
        text.setText(name);
    }

    inline void EventObject::setOnEnter(const std::string& name){
        EventObject::onEnter = name;
    }

    inline void EventObject::setOnLeave(const std::string& name){
        EventObject::onLeave = name;
    }

    inline const std::string& EventObject::getOnEnter() const{
        return EventObject::onEnter;
    }

    inline const std::string& EventObject::getOnLeave() const{
        return EventObject::onLeave;
    }

    inline bool EventObject::isInfinite() const{
        return EventObject::infinite;
    }

    inline void EventObject::setInfinite(bool infinite){
        EventObject::infinite = infinite;
    }

}



#endif
