#include <ChibiEngine/UserInterface/Slider.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Common/StringUtils.h>
#include <sstream>
#include <iomanip>

using namespace game;
using namespace std;
using namespace glm;

const string Slider::TYPE_NAME = "Slider";

Slider::Slider(SlotType* iter)
	:Slider(vec3(iter->x,iter->y,iter->z),
		iter->hw,
		iter->choiceValue.slider->min,
		iter->choiceValue.slider->max,
		iter->choiceValue.slider->value,
		iter->name,
		iter->choiceValue.slider->percision){
}

Slider::Slider(const glm::vec3& pos, float hw,
		float min, float max, float value, const std::string& name,int p)
	:min(min),max(max),value(value),
	 laf(Game::getLookAndFeel().slider),
	 percision(p>0?p:laf.defaultPercision),
	 text(Game::getLookAndFeel().font,toString(value,percision))
{
	setName(name);
	setPosition(pos);
	UIElement::setSize(vec2(hw,laf.hh));
	markX=getPositionByValue();
}


void Slider::setSize(const glm::vec2& size){
	UIElement::setSize(size);
	markX=getPositionByValue();
}

UIElement* Slider::containPoint(const glm::vec2& pnt){
	return isVisible() && isEnabled() &&
			pnt.x>getPosition().x+markX-laf.markLenX &&
			pnt.x<getPosition().x+markX+laf.markLenX &&
			pnt.y<getPosition().y+laf.markLenY &&
			pnt.y>getPosition().y-laf.markLenY? this : nullptr;
}

void Slider::setFloatValue(float val){
	value=val;
	text.setText(toString(value,percision));
	markX=getPositionByValue();
}

void Slider::processDraggingAction(const glm::vec2& delta){
	markX = markX+delta.x;
	if(markX<-getHW()+laf.markLenX){
		markX = -getHW()+laf.markLenX;
	}else if(markX>getHW()-laf.markLenX){
		markX=getHW()-laf.markLenX;
	}
	value=getValueByPosition();
	text.setText(toString(value,percision));
    Game::getUserInterface()->fireEvent(getName(),text.getText());
}

void Slider::setPosition(const glm::vec3& pos){
	UIElement::setPosition(pos);
	markX=getPositionByValue();
}

float Slider::getPositionByValue(){
	return (getHW()-laf.markLenX)*(2.0f*(value-min)/(max-min)-1.0f);
}

float Slider::getValueByPosition(){
	return (max-min)*(markX/(getHW()-laf.markLenX)+1.0f)/2.0f + min;
}

void Slider::draw(){
	const PrimitiveDrawer* brush = Game::getPrimitiveDrawer();
	brush->drawRectangleBorder(
            createMatrix(getPosition(), vec2(getHW(), laf.baseHH)), laf.recColor);
	brush->drawFilledRectangle(
            createMatrix(getPosition() + vec3(markX, 0, 0),vec2(laf.markLenX, laf.markLenY)),
                    isHovered() ? laf.hoveredColor : laf.markColor);
	text.draw(getPosition()+vec3(markX,0.0f,0.0f),laf.textColor);
}

void Slider::setValue(const std::string& value){
    setFloatValue(toFloat(value));
}

std::string Slider::getValue() const{
    return toString(value);
}
