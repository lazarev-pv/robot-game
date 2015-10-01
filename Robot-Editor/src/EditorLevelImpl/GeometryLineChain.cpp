#include <EditorLevelImpl/GeometryLineChain.h>

#include <ChibiEngine/XMLParser/WorldParser.h>
#include <EditorLevelImpl/EditorConstants.h>
#include <EditorLevel.h>

using namespace game;
using namespace std;
using namespace glm;


GeometryLineChain::GeometryLineChain(Layer* layer)
	:OperableObject(100,"",layer),
	 chainPresentation(Color::White),
     elements(),
    friction(1.0){
}

GeometryLineChain::GeometryLineChain(GeometrySlot* slot, Layer* layer)
	:OperableObject(100,slot->name,layer),
	 chainPresentation(Color::White),
     elements(),
    friction(slot->chain->friction){
	vector<vec3> pnts;
	pnts.reserve(slot->chain->point.size());
	int index = 0;
	for(auto point : slot->chain->point){
		vec3 pos(point->x, point->y,0);
		pnts.push_back(pos);
		elements.push_back(new GeometryLineChainElement(pos, this, index));
		index++;
	}
	chainPresentation.add(pnts);
}

GeometryLineChain::~GeometryLineChain(){
	for(auto el : elements){
		el->setNeedToUnbindOnDelete(false);
		delete el;
	}
}

void GeometryLineChain::Move(const glm::vec3& delta){}

void GeometryLineChain::grab(const glm::vec2& pnt){}

void GeometryLineChain::release(){}

glm::vec3 GeometryLineChain::getPosition() const{
	return vec3(0,0,0);
}

void GeometryLineChain::setPosition(const glm::vec3& position){
}

void GeometryLineChain::draw(CameraCHandle camera){
	chainPresentation.draw(camera);
	for(auto el: elements){
		el->draw(camera);
	}
}

OperableObject* GeometryLineChain::containsPoint(const glm::vec2& pnt){
    if(!EditorLevel::instance()->isSelectable(EditorConstants::GEOM_CHAIN))
        return nullptr;

    for(auto el : elements){
		OperableObject* res = el->containsPoint(pnt);
		if(res!=nullptr)
			return res;
	}

	return nullptr;
}

void GeometryLineChain::save2World(WorldType* world){

}

void GeometryLineChain::save2Layer(WorldLayers* layer) const{
    GeometrySlot* slot = new GeometrySlot();
    ERChainType* chainType = new ERChainType();
    slot->chain=chainType;
    for(auto el: elements){
        Point2D* pnt = new Point2D();
        pnt->x = el->getPosition().x;
        pnt->y = el->getPosition().y;

        chainType->point.push_back(pnt);
    }
    chainType->friction=friction;
    chainType->category = "phys";
    slot->name = getName();

    layer->geometry->slot.push_back(slot);
}

void GeometryLineChain::viewContextMenu(EditorLevel* level,const glm::vec2& pnt){
}

OperableObject* GeometryLineChain::clone(){
	return this;
}

void GeometryLineChain::add(GeometryLineChainElement* el){
	el->setIndex(elements.size());
	elements.push_back(el);
	chainPresentation.add(el->getPosition());
}

void GeometryLineChain::moveElement(int index, const glm::vec3& delta){
	chainPresentation.move(index, delta);
}
void GeometryLineChain::setElementPosition(int index, const glm::vec3& pos){
	chainPresentation.setPosition(index, pos);
}

void GeometryLineChain::deleteElement(int index){
	assert(index<static_cast<int>(elements.size()));

	chainPresentation.deleteElement(index);
	elements.erase(elements.begin()+index);
	for(size_t i = 0; i<elements.size();++i){
		elements[i]->setIndex(i);
	}
}

