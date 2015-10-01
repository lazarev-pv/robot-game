/*
 * UIElement.h
 *
 *  Created on: Apr 15, 2014
 *      Author: paul
 */

#ifndef __UIELEMENT_H_
#define __UIELEMENT_H_

#include <glm/glm.hpp>
#include <string>
#include <ChibiEngine/Input/Input.h>
#include <ChibiEngine/Common/ClassUtils.h>
#include <ftsizes.h>

namespace game{

	class UserInterface;
	class UIElement : private UniqueClass{
	public:
		UIElement();
		virtual ~UIElement();

		virtual void executeAction();
		virtual void processDraggingAction(const glm::vec2& delta);
		virtual void executePushOnAnimation();
		virtual void executePushOffAnimation();
		virtual void executeHoverOnAnimation();
		virtual void executeHoverOutAnimation();
        virtual bool executeScroll(bool up);
		virtual void executeFocused(){};
		virtual void executeLostFocus(){};
		virtual void executeViewCursor(bool ){};
		virtual void add2UI(UserInterface* ui);
		virtual UIElement* containPoint(const glm::vec2& pnt);
		virtual void onKeyPressed(KeyCode keyCode, bool special);
		virtual void draw(){};
		virtual void setVisible(bool flag);

        virtual void setValue(const std::string& ){};
        virtual std::string getValue() const {return "";};

		virtual const std::string& getTypeName(){
			return TYPE_NAME;
		}

		const glm::vec3& getPosition() const;
		bool isVisible();
		bool isEnabled();
		const std::string& getName() const;
		bool isInstantAction();
		glm::vec2 getSize();
		float getHW();
		float getHH();

		virtual void setPosition(const glm::vec3& pos);
		virtual void setSize(const glm::vec2& size);
		virtual void move(const glm::vec3 &delta);

		inline void setEnabled(bool value);
		inline void setName(const std::string& name);

        bool isHovered() const{
            return hovered;
        }
	protected:
		void setInstantAction(bool flag){
			instantAction = flag;
		}

        bool isManagedByUI() const{
            return managedByUI;
        }

	private:
		const static std::string TYPE_NAME;

		struct UIElemParms{

			/* negative length means autosize */
			UIElemParms();
			UIElemParms(const std::string& name,
					float x, float y, float z,
					float hw, float hh,
					bool visible,
					bool enabled);

			bool containPoint(const glm::vec2& pnt){
				return visible && enabled &&
						pnt.x>pos.x-hw &&
						pnt.x<pos.x+hw &&
						pnt.y<pos.y+hh &&
						pnt.y>pos.y-hh;
			}

			glm::vec3 pos;
			float hw,hh;
			std::string name;
			bool visible;
			bool enabled;
		};

		UIElemParms parms;
		bool instantAction;
        bool managedByUI;
        bool hovered;
	};


    inline void UIElement::setName(const std::string& name){
        parms.name = name;
    }

    inline void UIElement::setEnabled(bool flag){
        parms.enabled = flag;
    }

}

#endif /* UIELEMENT_H_ */
