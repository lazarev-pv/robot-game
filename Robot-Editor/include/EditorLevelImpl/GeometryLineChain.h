#ifndef __GEOMETRYLINECHAIN_H_
#define __GEOMETRYLINECHAIN_H_


#include <EditorLevelImpl/OperableObject.h>
#include <ChibiEngine/Render/Primitives/LinesChain.h>
#include <vector>
#include <ChibiEngine/XMLParser/WorldParser.h>

class GeometrySlot;
class WorldLayers;
namespace game{

class Layer;
class GeometryLineChain;
class GeometryLineChainElement : public OperableObject{
public:
	GeometryLineChainElement(const glm::vec3& pos,GeometryLineChain* chain, int index=0);
	~GeometryLineChainElement();

	void setIndex(int index){
		this->index = index;
	}

	void setNeedToUnbindOnDelete(bool _need2Unbind){
		need2Unbind = _need2Unbind;
	}

	virtual void Move(const glm::vec3& delta);
	virtual void grab(const glm::vec2& pnt);
	virtual void release();
	virtual void draw(CameraCHandle camera);
	virtual OperableObject* containsPoint(const glm::vec2& pnt);
	virtual void save2World(WorldType* world);
	virtual void viewContextMenu(EditorLevel* level,const glm::vec2& pnt);
	virtual glm::vec3 getPosition() const;
	virtual void setPosition(const glm::vec3& position);
	virtual OperableObject* clone();
    virtual void save2Layer(WorldLayers* layer) const{};

    virtual void setSelected(bool isSelected);
private:
	glm::vec3 pos;
	int index;
	bool grabbed;
	bool need2Unbind;
	GeometryLineChain* chain;

};

class GeometryLineChain : public OperableObject{
public:
	GeometryLineChain(Layer* layer);
	GeometryLineChain(GeometrySlot* slot, Layer* layer);

	~GeometryLineChain();

	void add(GeometryLineChainElement* el);
	void moveElement(int index, const glm::vec3& delta);
	void setElementPosition(int index, const glm::vec3& pos);
	void deleteElement(int index);
    inline void setFriction(float friction);
    inline float getFriction() const;

	virtual void Move(const glm::vec3& delta);
	virtual void grab(const glm::vec2& pnt);
	virtual void release();
	virtual void draw(CameraCHandle camera);
	virtual OperableObject* containsPoint(const glm::vec2& pnt);

    virtual void save2World(WorldType* world);
    virtual void save2Layer(WorldLayers* layer) const;
	virtual void viewContextMenu(EditorLevel* level,const glm::vec2& pnt);
	virtual glm::vec3 getPosition() const;
	virtual void setPosition(const glm::vec3& position);
	virtual OperableObject* clone();

private:
    LinesChain chainPresentation;
    std::vector<GeometryLineChainElement*> elements;
    float friction;
};


    inline void GeometryLineChain::setFriction(float friction){
        GeometryLineChain::friction = friction;
    }

    inline float GeometryLineChain::getFriction() const{
        return GeometryLineChain::friction;
    }

}



#endif
