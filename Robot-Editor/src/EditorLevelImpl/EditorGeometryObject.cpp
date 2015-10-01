
#include <EditorLevelImpl/EditorGeometryObject.h>
#include <EditorLevelImpl/EditorPres2D.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/UserInterface/ContextMenu.h>
#include <EditorLevelImpl/EditorConstants.h>

using namespace game;
using namespace std;
using namespace glm;

std::unordered_map<std::string, int> EditorGeometryObject::STR2INT_REFERENCE
        ({{"static",0},{"kinematic",1},{"dynamic",2}});

std::unordered_map<int,std::string> EditorGeometryObject::INT2STR_REFERENCE
        ({{0,"static"},{1,"kinematic"},{2,"dynamic"}});

EditorGeometryObject::EditorGeometryObject(glm::vec3 pos,
        std::string name,
        Layer* layer,
        int physType)
    :PointObject(pos,name,layer),physType(physType){
}

EditorGeometryObject::EditorGeometryObject(GeometrySlot* slot, Layer* layer)
    :EditorGeometryObject(vec3(slot->x,slot->y,0),slot->name,layer,slot->physType){
    for(auto iter : slot->box){
        add(new EditorFixtureObject(iter,this,layer));
    }
}

EditorGeometryObject::~EditorGeometryObject(){
}


void EditorGeometryObject::save2Layer(WorldLayers* layer) const {
    GeometrySlot *slot = new GeometrySlot();
    for(auto iter : elements) {
        slot->box.push_back(iter->save());
    }
    slot->name = getName();
    slot->x=getPosition().x;
    slot->y=getPosition().y;
    slot->physType = physType;
    layer->geometry->slot.push_back(slot);
}

void EditorGeometryObject::setSelected(bool isSelected){
    PointObject::setSelected(isSelected);
    if(isSelected)
        EditorLevel::instance()->selectGeomObject(this);
}

void EditorGeometryObject::draw(CameraCHandle camera){
    for(auto iter:elements){
        iter->draw(camera);
    }
    PointObject::draw(camera);
}

void EditorGeometryObject::Move(const glm::vec3& delta){
    PointObject::Move(delta);
    for(auto iter:elements){
        iter->forceMove(delta);
    }
   /* for(auto iter : elements){
        // Не используем move потому что он заточен под передвижение пользователем а не объекто владельцем
        iter->setPosition(iter->getPosition()+delta);
    }*/
}

void EditorGeometryObject::setPosition(const glm::vec3& position){
    Move(position - getPosition());
}

OperableObject* EditorGeometryObject::containsPoint(const glm::vec2& pnt){
    if(!EditorLevel::instance()->isSelectable(EditorConstants::GEOM_OBJECT))
        return nullptr;

    OperableObject* obj = PointObject::containsPoint(pnt);
    if(obj!=nullptr){
        return obj;
    }

    for(auto el : elements){
        OperableObject* res = el->containsPoint(pnt);
        if(res!=nullptr)
            return res;
    }

    return nullptr;
}

void EditorGeometryObject::add(EditorFixtureObject* fixture){
    elements.push_back(fixture);
}

EditorFixtureObject::EditorFixtureObject(
        glm::vec3 pos,
        EditorGeometryObject* parent,
        Layer* layer,
        glm::vec2 size,
        float friction, float density,
        const std::string& category,
        bool sensor)
:BoxAreaObject(pos+parent->getPosition(),size,EditorConstants::GEOM_OBJECT,0,"",layer),
 density(density),
 friction(friction),
 category(category),
 parent(parent),
 sensor(sensor){
}

EditorFixtureObject::EditorFixtureObject(
        ERBoxType* slot,
        EditorGeometryObject* parent,
        Layer* layer)
:EditorFixtureObject(vec3(slot->x,slot->y,0),
                      parent,
                      layer,
                      vec2(slot->hw, slot->hh),
                      slot->friction,
                      slot->density,
                      slot->category.size()>0?slot->category:CategoryReference::PHYS,
                      slot->sensor){
    for (auto iter : slot->art) {
        presentations.push_back(new EditorPres2D(iter, this, layer));
    }
}

OperableObject*EditorFixtureObject::clone(){
	EditorFixtureObject * obj = new EditorFixtureObject(
            getPosition()-parent->getPosition(),
            parent,
            getLayer(),
            getSize(),
            friction,density,
            category);

	for(auto pres : presentations){
        EditorPres2D* newPres = new EditorPres2D(pres->getPosition()-obj->getPosition(),pres->getSpriteName(),pres->getScale(),obj,getLayer());
        newPres->setRotation(pres->getRotation());
		 obj->addPresentation(newPres);
	}
    parent->add(obj);
	return obj;
}

EditorFixtureObject::~EditorFixtureObject(){
	deleteAll(presentations);
}

void EditorFixtureObject::forceMove(const glm::vec3& delta){
    BoxAreaObject::setPosition(getPosition()+delta);
    for(auto iter : presentations){
        // Не используем move потому что он заточен под передвижение пользователем а не объекто владельцем
        iter->setPosition(iter->getPosition()+delta);
    }
}


void EditorFixtureObject::Move(const glm::vec3& delta){
    BoxAreaObject::Move(delta);
    for(auto iter : presentations){
        // Не используем move потому что он заточен под передвижение пользователем а не объекто владельцем
        iter->setPosition(iter->getPosition()+delta);
    }
}

void EditorFixtureObject::draw(CameraCHandle camera){
	for(auto iter:presentations){
		iter->draw(camera);
	}
    BoxAreaObject::draw(camera);
}

void EditorFixtureObject::addPresentation(EditorPres2D* pres){
	presentations.push_back(pres);
}

void EditorFixtureObject::removePresentation(EditorPres2D* pres){
	presentations.erase(std::remove(presentations.begin(), presentations.end(), pres), presentations.end());
}

OperableObject*EditorFixtureObject::containsPoint(const glm::vec2& pnt){
    for(auto iter: presentations){
        OperableObject* obj = iter->containsPoint(pnt);
        if(obj!=nullptr)
            return obj;
    }

    return BoxAreaObject::containsPoint(pnt);
}

void EditorFixtureObject::setPosition(const glm::vec3& position){
    Move(position-getPosition());
}

void EditorFixtureObject::setSelected(bool isSelected){
    BoxAreaObject::setSelected(isSelected);
    if(isSelected)
        EditorLevel::instance()->selectFixtureObject(this);
}

ERBoxType* EditorFixtureObject::save() const{
    ERBoxType *et = new ERBoxType();
    for(auto iter:this->presentations){
        et->art.push_back(iter->save2ArtType());
    }
    et->hw = getSize().x;
    et->hh = getSize().y;
    et->x = getPosition().x-parent->getPosition().x;
    et->y = getPosition().y-parent->getPosition().y;
    et->density = density;
    et->friction = friction;
    et->category = getCategory();
    et->sensor = sensor;
    return et;
}

void EditorGeometryObject::setPhysType(const std::string& name){
    physType = STR2INT_REFERENCE[name];
}

std::string& EditorGeometryObject::getPhysType(){
    return INT2STR_REFERENCE[physType];
}

