#ifndef __INPUTFIELDS_CONTAINER_
#define __INPUTFIELDS_CONTAINER_

#include <string>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/Game.h>

#include <ChibiEngine/Log/Logger.h>

namespace game{

class EditorLevel;

template <class OBJ, class LEVEL>
class FieldsContainer {
public:
	FieldsContainer(
			const std::vector<std::string>& parms,
			const std::string& eventName,
			LEVEL* level):
		obj(nullptr),
        fields(0),
		level(level),
		byMouse([this,eventName](const std::string& evName){
			if(evName==eventName) updateFields();
		}){

		UserInterface* ui = Game::getUserInterface();
		for(auto parm : parms){
			fields.push_back(ui->findElement(parm));
			ui->addEventListener(parm,std::bind(&FieldsContainer::fieldsChanged, this, std::placeholders::_1));
		}

	};

	virtual ~FieldsContainer() = default;

	void link(OBJ* _obj){
		if(obj!=nullptr)
			obj->removeEventListener(evHandle);
		obj=_obj;
		if(obj!=nullptr){
			evHandle = obj->addEventListener(byMouse);
            updateFields();
        }else{
			for(auto field : fields){
				field->setValue("");
			}
		}
	}
	const OBJ* getObject(){
		return obj;
	}
protected:
	virtual void updateObject() = 0;
	virtual void updateFields() = 0;
	OBJ* obj;
	std::vector<UIElement*> fields;
	LEVEL* level;
private:

	void fieldsChanged(const std::string& name){
		if(obj!=nullptr){
            obj->setSilentMode(true);
            updateObject();
            obj->setSilentMode(false);
        }
	}

	EventSupportable::EventListener byMouse;
	EventSupportable::EventListenerHandle evHandle{};
};

}



#endif
