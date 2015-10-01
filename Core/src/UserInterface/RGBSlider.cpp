#include <ChibiEngine/UserInterface/RGBSlider.h>
#include <ChibiEngine/UserInterface/Slider.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/UserInterface/LookAndFeel.h>
#include <ChibiEngine/Common/StringUtils.h>
#include <sstream>
#include <iomanip>

using namespace game;
using namespace std;
using namespace glm;

const string RGBSlider::TYPE_NAME = "RGBSlider";

RGBSlider::RGBSlider(SlotType* iter){
	const SliderLAF& laf = Game::getLookAndFeel().slider;
	const RGBSliderLAF& rgbLaf = Game::getLookAndFeel().rgb;
	colorHW = rgbLaf.colorHW;

	value=game::getColor(iter->choiceValue.rgb->color,nullptr);

	vec3 pos(iter->x,iter->y,iter->z);
	setPosition(pos);
	setSize(vec2(iter->hw,4*laf.hh));
	setName(iter->name);

	rSlider = new Slider(pos+vec3(0,laf.hh*3.0f,0.2f),
			iter->hw,0.0f,1.0f, value.toVector().r,iter->name+".r",2);
	gSlider = new Slider(pos+vec3(0,laf.hh,0.2f),
			iter->hw,0.0f,1.0f, value.toVector().g,iter->name+".g",2);

	bSlider = new Slider(pos+vec3(0,-laf.hh,0.2f),
			iter->hw,0.0f,1.0f, value.toVector().b,iter->name+".b",2);
	aSlider = new Slider(pos+vec3(0,-laf.hh*3.0f,0.2f),
			iter->hw,0.0f,1.0f, value.toVector().a,iter->name+".a",2);

	add(rSlider);
	add(gSlider);
	add(bSlider);
	add(aSlider);

	UserInterface* ui = Game::getUserInterface();
	ui->addEventListener(iter->name+".r", [this](const std::string& name){
		value.setRed(toFloat(name));
		Game::getUserInterface()->fireEvent(getName(), value.toString());
	});
	ui->addEventListener(iter->name+".g", [this](const std::string& name){
		value.setGreen(toFloat(name));
		Game::getUserInterface()->fireEvent(getName(), value.toString());
	});
	ui->addEventListener(iter->name+".b", [this](const std::string& name){
		value.setBlue(toFloat(name));
		Game::getUserInterface()->fireEvent(getName(), value.toString());
	});
	ui->addEventListener(iter->name+".a",[this](const std::string& name){
		value.setAlpha(toFloat(name));
		Game::getUserInterface()->fireEvent(getName(), value.toString());
	});
}

void RGBSlider::setSize(const glm::vec2& size){
	ElementCollection::setSize(size);
    setSize2AllSlots(size);
}

void RGBSlider::draw(){
	Game::getPrimitiveDrawer()->
			drawFilledRectangle(
            createMatrix(getPosition(), getSize()), value);
}

void RGBSlider::setPosition(const glm::vec3& pos){
	ElementCollection::setPosition(pos);
}

void RGBSlider::setColor(const Color& val){
	rSlider->setFloatValue(val.toVector().r);
	gSlider->setFloatValue(val.toVector().g);
	bSlider->setFloatValue(val.toVector().b);
	aSlider->setFloatValue(val.toVector().a);
	value=val;
}

void RGBSlider::setValue(const std::string& value){
    RGBSlider::value = parseFromToString(value);
}

std::string RGBSlider::getValue() const{
    return value.toString();
}