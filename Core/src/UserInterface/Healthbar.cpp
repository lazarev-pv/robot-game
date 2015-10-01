#include <ChibiEngine/UserInterface/Healthbar.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Resource/Resources.h>
#include <sstream>
#include <iomanip>
#include <ChibiEngine/Common/StringUtils.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/UserInterface/LookAndFeel.h>
#include <ChibiEngine/Render/Sprites/NineSliceSprite.h>
#include <ChibiEngine/Render/ScreenSystem.h>

#define GLM_FORCE_RADIANS
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtx/transform.hpp>

using namespace game;
using namespace std;
using namespace glm;

const string Healthbar::TYPE_NAME = "Healthbar";

void Healthbar::setValue(const std::string& value){
    setFloatValue(toFloat(value));
}

std::string Healthbar::getValue() const{
    return toString(getFloatValue());
}

Healthbar::Healthbar(float min, float max, float value, const Color& color,float hw) :
    laf(Game::getLookAndFeel().healthbar),
    min(min),max(max),value(value),
    color(color),
    back(laf.backImage,
         hw/Game::getScreen()->getWidthDensity(),
         laf.hh/Game::getScreen()->getHeightDensityNoAspect()){

    UIElement::setSize(vec2(hw,laf.hh));
}

Healthbar::Healthbar(SlotType* iter):
    Healthbar(iter->choiceValue.healthbar->min,
            iter->choiceValue.healthbar->max,
            iter->choiceValue.healthbar->value,
            parse(iter->choiceValue.healthbar->color),
            iter->hw){

    setName(iter->name);
    setPosition(vec3(iter->x,iter->y,iter->z));
}


void Healthbar::draw(){

    back.draw(getPosition());
    {
        float barHW = (getSize().x-laf.padding.x)*(value-min)/(max-min);
        float barHH = (getSize().y-laf.padding.y);
        vec3 pos = getPosition();
        pos.x+=-getSize().x+laf.padding.x+barHW;
        glm::mat4 transMat = glm::translate(pos)
                * glm::scale(mat4(1.0f),vec3(vec2(barHW,barHH)/laf.image->getSize(),1.0f));

         Game::getShaderSystem()->get("healthbar")
                .draw(laf.image->getShaderValue(),
                        unordered_map<string, UniformValue>({
                {"u_color",UniformValue(color)},
                {"u_mvp_matrix",UniformValue(transMat)}}));
    }

}








