#include <ChibiEngine/Game.h>
#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Input/Input.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/UserInterface/InputField.h>

using namespace game;
using namespace std;
using namespace glm;

const string InputField::TYPE_NAME = "InputField";

const unsigned char ENTER_CODE = 13;
const unsigned char BACKSPACE_CODE = 8;

InputField::AbstractDrawer::AbstractDrawer(InputField* parent):
		parent(parent),
		laf(Game::getLookAndFeel().inputField){
}

glm::vec3 InputField::AbstractDrawer::getTextPos(){
	float offset = parent->getHW() - (parent->textPart.getWorldHW()+laf.padding.x)*Game::getScreen()->getWidthDensity();
	return parent->getPosition()-vec3(offset,0,parent->getPosition().z);
}

InputField::SimpleDrawer::SimpleDrawer(InputField* parent):
		AbstractDrawer(parent){
}

void InputField::SimpleDrawer::draw(){
	Game::getPrimitiveDrawer()->drawBorderedFilledRectangle(
			vec3(parent->getPosition().x,parent->getPosition().y,0),
			parent->getSize(),
            laf.borderThickness,
            parent->isEnabled() ? laf.backColor : laf.disBackColor,
			parent->focused ? laf.selectedBorderColor : laf.borderColor);

	vec3 textPos = getTextPos();

	if(parent->selection){
		Game::getPrimitiveDrawer()->drawFilledRectangle(
				createMatrix(textPos,vec2(parent->textPart.getScreenHWidth(),parent->textPart.getScreenHHeight())),
				laf.selectColor);
	}

	parent->textPart.draw(textPos, laf.font.color);

	if(parent->viewCursor){
		float charWidth = (parent->value.size()>0)?(parent->textPart.getScreenHWidth()*2)/parent->value.size():0;
		float charXOffset = charWidth * parent->cursorPosition - parent->textPart.getScreenHWidth();
		Game::getPrimitiveDrawer()->drawFilledRectangle(
				createMatrix(
                        textPos + vec3(charXOffset, 0, 0),
				        vec2(laf.cursorHW * Game::getScreen()->getWidthDensity(), parent->getHH() - laf.borderThickness)),
				laf.cursorColor);
	}
}

void InputField::executeHoverOnAnimation(){
    UIElement::executeHoverOnAnimation();
	Game::getInputSystem()->setMouseCursor(Cursor::TEXT);
}
void InputField::executeHoverOutAnimation(){
    UIElement::executeHoverOutAnimation();
	Game::getInputSystem()->setMouseCursor(Cursor::USUAL);
}

size_t getMaxSize(SlotType* iter){
	if(iter==nullptr){
		return 10;
	}
	const InputSlotUnion& u = iter->choiceValue.input->choiceValue;
	if(u.all!=nullptr){
		return static_cast<size_t>(u.all->maxSize);
	}else if(u.onlyDigits!=nullptr){
		return static_cast<size_t>(u.onlyDigits->maxSize);
	}else{
		return 10;
	}
}

std::function<bool(char, const std::string&)> createFilter(SlotType* iter){
	size_t maxSize = getMaxSize(iter);
	if(iter==nullptr || iter->choiceValue.input->choiceValue.all!=nullptr){
		return [maxSize](char , const std::string& value){
			return value.size()+1 <= maxSize;
		};
	}else{
		return [maxSize](char newChar, const std::string& value){
			if(value.size()+1>maxSize)
				return false;
			return (newChar>='0' && newChar<='9') || newChar == '-' || newChar =='.';
		};
	}
}


InputField::InputField(SlotType* iter):
		filter(createFilter(iter)),
		maxSize(getMaxSize(iter)),
		value(iter->choiceValue.input->value.substr(0, maxSize)),
		textPart(Game::getLookAndFeel().inputField.font, value/*"!Ay"*/),
		drawer(this){

	setPosition(vec3(iter->x,iter->y,iter->z));
	setName(iter->name);

	const InputFieldLAF& laf = Game::getLookAndFeel().inputField;
	setSize(vec2(iter->hw>0?iter->hw:getHW(),
			iter->hh>0?iter->hh:(
                    Game::getLookAndFeel().inputField.font.font->getAtlas(
                            Game::getLookAndFeel().inputField.font.size
                    ).maxH+laf.padding.y+laf.padding.w)*Game::getScreen()->getHeightDensity()));
}

InputField::InputField(const std::string& _value)
	:filter(createFilter(nullptr)),
	 maxSize(getMaxSize(nullptr)),
	 value(_value),
	 textPart(Game::getLookAndFeel().inputField.font, value),
	 drawer(this){

	const InputFieldLAF& laf = Game::getLookAndFeel().inputField;
	setSize(vec2(getHW(),
			(Game::getLookAndFeel().inputField.font.font->getAtlas(
                    Game::getLookAndFeel().inputField.font.size
            ).maxH+laf.padding.y+laf.padding.w)*Game::getScreen()->getHeightDensity()));
}

void InputField::acceptCode(unsigned char code){
	if(selection){
        setValue(string(1,code));
		selection = false;
		return;
	}

	if(filter==nullptr || filter(code,value)){
		value.insert(cursorPosition,string(1,code));
		moveCursor(1);
	}
}


void InputField::onKeyPressed(KeyCode keyCode, bool special){
	if(special){
		if(keyCode == RawEventListener::LEFT_ARROW){
			moveCursor(-1);
		}else if(keyCode == RawEventListener::RIGHT_ARROW){
			moveCursor(1);
		}
		return;
	}

	if(keyCode==ENTER_CODE){
        Game::getUserInterface()->fireEvent(getName(),value);
	}else if(keyCode==BACKSPACE_CODE){
		deleteLast();
		textPart.setText(value);
	}else{
		acceptCode(keyCode);
		textPart.setText(value);
	}
}

void InputField::executeLostFocus(){
	viewCursor = false;
	selection = false;
	focused = false;
}

void InputField::executeFocused(){
	selection = true;
	focused = true;
}

void InputField::deleteLast(){
	if(cursorPosition>0 && value.size()>0){
		value.erase(cursorPosition-1, 1);
		moveCursor(-1);
	}
}

void InputField::moveCursor(int delta){
	selection = false;

	if(delta>0 && cursorPosition< static_cast<int>(value.size())){
		cursorPosition++;
	}else if(delta < 0 && cursorPosition>0){
		cursorPosition--;
	}
}


void InputField::setValue(const std::string& _value){
    value = _value.substr(0, maxSize);
	cursorPosition = static_cast<int>(value.size());
	textPart.setText(value);
}

void InputField::draw(){
	drawer.draw();
}

void InputField::executeViewCursor(bool visible){
	viewCursor = visible;
}
