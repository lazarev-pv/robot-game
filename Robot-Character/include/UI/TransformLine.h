#ifndef __ROBOT_CHARACTER_TRANSFORM_LINE_H_
#define __ROBOT_CHARACTER_TRANSFORM_LINE_H_


#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include <UI/CurveKey.h>
#include <UI/TransformKey.h>
#include <ChibiEngine/Input/Input.h>
#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChedConstants.h>

namespace game{

    class TransformLabel;
    class TransformLinesGroup;
    class TransformLine : public UIElement{
    public:
        //Собсвенные методы и общие с Header-ом
        inline void addMark(TransformKey::Time time);
        void clear();
        void calcTimeMarks(float startX, float endX, int divStep, int markFreq);
        void processHighlight(const glm::vec2& pnt);
        inline const std::map<TransformKey::Time,TransformKey>& getKeys();
        TransformKey::Time calcMaxTime() const;
        void try2HookKey(const glm::vec2& mousePnt);
        void releaseKey(const glm::vec2& mousePnt);
        inline TransformLabel* getLabel() const;

        // Методы которых нет в Header
        void setSelected(bool selected);
        void add(const glm::vec2& delta, const glm::vec2& currPnt, TransformKey::Time currentTime);
        void select(TransformKey::Time time);
        void deleteKey(TransformKey::Time key);
        void copyKey(TransformKey::Time key);

        // От UIElement
        void executePushOnAnimation() override;
        void executePushOffAnimation() override;
        void onKeyPressed(KeyCode keyCode, bool special) override;
        void executeAction() override;

        virtual void setVisible(bool flag) override;

        void draw() override;

    protected:
        TransformLine(TransformLinesGroup* parent,
                const std::string& name,
                const std::string& icon,
                const Color& markColor,
                const glm::vec3& pos,
                const glm::vec2& size,
                TransformId transformId);

        TransformKey::Time findKey(const glm::vec2& pnt);

        virtual void onKeyDelete(TransformKey::Time time) = 0;
        virtual void onKeyModify(const glm::vec2& delta, const glm::vec2& currPnt, TransformKey::Time currentTime) = 0;
        virtual void onMoveKey(TransformKey::Time oldTime, TransformKey::Time newTime) = 0;
        virtual void onCopyKey(TransformKey::Time oldTime, TransformKey::Time newTime) = 0;

        std::map<TransformKey::Time,TransformKey> keys;
        TransformKey::Time selectedKeyTime = -1;
        TransformKey::Time copyedKeyTime = -1;

        TransformLinesGroup* parent;
        Color bgColor;
        bool drawCurves = true;

    private:
        TransformKey::Time getTimeByMouse(const glm::vec2& pnt);

        ShaderValues sv;
        bool selected = false;
        Color keyColor;

        TransformKey hookedKey;

        bool keyHooked = false;
        TransformId transformId;
        TransformLabel* label;
    };


    inline const std::map<int,TransformKey>& TransformLine::getKeys(){
        return keys;
    }

    inline void TransformLine::addMark(int time){
        keys.insert({time,TransformKey(time)});
    }

    inline TransformLabel* TransformLine::getLabel() const{
        return label;
    }
}


#endif