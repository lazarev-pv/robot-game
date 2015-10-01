#include <ChibiEngine/UserInterface/TextLabel.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Render/ScreenSystem.h>

using namespace game;
using namespace std;
using namespace glm;

const string TextLabel::TYPE_NAME = "TextLabel";

TextLabel::TextLabel(SlotType* iter):
		UIElement(),
		spriteText(Game::getLookAndFeel().font, iter->choiceValue.text->text),
		color(Game::getLookAndFeel().font.color){
		setName(iter->name);
		setPosition(vec3(iter->x,iter->y,iter->z));
		setSize(vec2(iter->hw>0?iter->hw:spriteText.getScreenHWidth(),
				iter->hh>0?iter->hh:spriteText.getScreenHHeight()));
}

TextLabel::TextLabel(const std::string& text)
: UIElement(),
  spriteText(Game::getLookAndFeel().font, text),
  color(Game::getLookAndFeel().font.color){

	vec4 padding = Game::getLookAndFeel().defaultPadding;

    setSize(SpriteText::toScreenSize(vec2(
            spriteText.getWorldHW()+(padding.x+padding.z),
            spriteText.getWorldHH()+(padding.y+padding.w)
    )));
}

void TextLabel::draw(){
	draw(getPosition());
}

void TextLabel::draw(const glm::vec3& position){
	spriteText.draw(position,color);
}


void TextLabel::setValue(const std::string& v){
    spriteText.setText(v);
    setSize(vec2(spriteText.getScreenHWidth(),spriteText.getScreenHHeight()));
}

std::string TextLabel::getValue() const{
    return spriteText.getText();
}




