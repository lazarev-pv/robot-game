#ifndef __ROBOT_CHARACTER_INSTRUMENT_HELPER_
#define __ROBOT_CHARACTER_INSTRUMENT_HELPER_

#include <glm/glm.hpp>
#include <map>
#include <ChedConstants.h>
#include "ChedObject.h"


namespace game{
    class ChedBone;
    class TexturedObject;

    /** Помошник при работе с текущим инструментом:
    * -) Отображает визуально текущий инструмент: вращение, перемещение и.п.
    * -) Для некотрых инструментов добавляет возможность ограничивать перемешения мыши */

     class InstrumentHelper {
    public:
        InstrumentHelper();
        void attach2Bone(ChedObject* bone);
        void setCurrentTransform(TransformId value);
        inline void draw() const;
        inline glm::vec2 constrain(const glm::vec2& grabPnt, const glm::vec2& delta) const;
        inline void processHover(const glm::vec2& pnt);
        inline void setMouseDragging(bool value);
        inline void setVisible(bool value);
        inline TransformId getActiveTransform() const;
    private:

        class Strategy{
        public:
            inline Strategy(InstrumentHelper* parent);
            virtual ~Strategy() = default;
            virtual void draw() const = 0;
            virtual glm::vec2 constrain(const glm::vec2& grabPnt, const glm::vec2& delta) const = 0;
            virtual void processHover(const glm::vec2& pnt) = 0;
        protected:
            InstrumentHelper* parent;
        };

        class Rotator:public Strategy{
        public:
            Rotator(InstrumentHelper* parent);
            void draw() const override;
            glm::vec2 constrain(const glm::vec2& grabPnt, const glm::vec2& delta) const override;
            void processHover(const glm::vec2& pnt) override;
        private:
            TexturedObject* obj;
        };

        class Mover:public Strategy{
        public:
            Mover(InstrumentHelper* parent);
            void draw() const override;
            glm::vec2 constrain(const glm::vec2& grabPnt, const glm::vec2& delta) const override;
            void processHover(const glm::vec2& pnt) override;
        protected:
            Mover(InstrumentHelper* parent, const std::string& pres);

            TexturedObject* obj;
            bool xConstraintActive = false;
            bool yConstraintActive = false;
        };

         class Scaler:public Mover{
         public:
             Scaler(InstrumentHelper* parent);
         };

        Mover mover;
        Rotator rotator;
        Scaler scaler;
        std::map<TransformId, Strategy*> ref;
        Strategy* activeStrategy = nullptr;
        TransformId activeTransform = NoTransform;
        ChedObject* bone = nullptr;
        bool mouseDragging = false;
        bool visible = true;
    };

    inline InstrumentHelper::Strategy::Strategy(InstrumentHelper* parent):
            parent(parent){
    }

    inline void InstrumentHelper::draw() const{
        if(activeStrategy && bone && visible)
            activeStrategy->draw();
    }

    inline glm::vec2 InstrumentHelper::constrain(const glm::vec2& grabPnt, const glm::vec2& delta) const{
        if(activeStrategy && bone) {
            return activeStrategy->constrain(grabPnt, delta);
        }else{
            return delta;
        }
    }

    inline void InstrumentHelper::processHover(const glm::vec2& pnt){
        if(activeStrategy && bone && visible) {
            activeStrategy->processHover(pnt);
        }
    }

    inline void InstrumentHelper::setMouseDragging(bool value){
        mouseDragging = value;
    }

    inline void InstrumentHelper::setVisible(bool value){
        visible = value;
    }

    inline TransformId InstrumentHelper::getActiveTransform() const{
        return activeTransform;
    }
}

#endif