#ifndef __CHECK_BOX_H_
#define __CHECK_BOX_H_

#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/UserInterface/UserInterface.h>

class SlotType;
namespace game {

	class Checkbox : public UIElement {
	public:

        Checkbox(SlotType* iter);
        Checkbox(const glm::vec3& pos, const std::string& name, bool value);

        inline void setChecked(bool val);
        inline void setValue(const std::string& value) override;
        void draw() override;
        void executeAction() override;

        inline virtual const std::string& getTypeName();
        inline virtual std::string getValue() const;
        inline bool isChecked();

	private:
        bool value;
        std::string script;
		const static std::string TYPE_NAME;
	};

    void Checkbox::setChecked(bool val){
        value=val;
    }

    bool Checkbox::isChecked(){
        return value;
    }

    const std::string& Checkbox::getTypeName(){
        return TYPE_NAME;
    }

    inline void Checkbox::setValue(const std::string& value){
        Checkbox::value=value=="true";    
    }

    inline std::string Checkbox::getValue() const{
        return value?"true":"false";
    }
}



#endif
