#include <ChibiEngine/UserInterface/Dialog.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Render/Sprites/NineSliceSprite.h>

using namespace game;
using namespace std;
using namespace glm;

const string Dialog::TYPE_NAME = "Dialog";

class Dialog::Capture : public UIElement{
public:
	Capture(vec3 pos, float hw, float hh, const string& text, const DialogLAF& laf, Dialog* parent)
		:spriteText(laf.capture.font,text),
		 laf(laf),
		 parent(parent){
		setPosition(pos+vec3(0,0,0.05));
		setSize(vec2(hw,hh));
	}

	virtual const std::string& getTypeName(){
		return TYPE_NAME;
	}

	virtual void executeHoverOnAnimation(){
        UIElement::executeHoverOnAnimation();
		Game::getInputSystem()->setMouseCursor(Cursor::HAND);
	}
	virtual void executeHoverOutAnimation(){
        UIElement::executeHoverOutAnimation();
		Game::getInputSystem()->setMouseCursor(Cursor::USUAL);
	}
	virtual void draw(){
			//if(parent->sprite==nullptr){
				Game::getPrimitiveDrawer()->drawFilledRectangle(
                        createMatrix(getPosition(),getSize()),
                        laf.capture.color);
			//}
			spriteText.draw(getPosition(),laf.capture.font.color);
	}

	void processDraggingAction(const glm::vec2& delta){
		parent->move(vec3(delta, 0));
	}
private:
	const static std::string TYPE_NAME;

	SpriteText spriteText;
	const DialogLAF& laf;
	Dialog* parent;
};

const string Dialog::Capture::TYPE_NAME = "Capture";


Dialog::Dialog(SlotType* iter)
	:laf(Game::getLookAndFeel().dialogs){

	setName(iter->name);
	setSize(vec2(iter->hw,iter->hh));
	UIElement::setPosition(vec3(iter->x,iter->y,iter->z-0.1));
	float captureHHeight = laf.capture.height;

	Button* exitButton = new Button("x", vec3(0,0,0), vec2(-1,-1), iter->name);

	const vec4& padding = Game::getLookAndFeel().defaultPadding;
	if(captureHHeight<=0){
		captureHHeight = exitButton->getHH()+(padding.y+padding.w)/Game::getScreen()->getWidth();
		vec3 exitButtonPosition= getPosition()+vec3(getHW() - exitButton->getHW(),getHH()+captureHHeight,0.1);
		exitButton->setPosition(exitButtonPosition);
		exitButton->setName(iter->name+".close");
	}

	Capture* cap = new Capture(
			getPosition()+vec3(0,getHH()+captureHHeight,0),
			getHW(),captureHHeight,
			iter->choiceValue.dialog->capture,
			laf, this);

	add(cap);
	add(exitButton);

    Game::getUserInterface()->addEventListener(exitButton->getName(),
			[this](const std::string& name){
				if(name=="x"){
					setVisible(false);
				}
			});

	if(laf.image.size()>0){
		sprite.reset(new NineSliceSprite(
				Game::getResources()->findSpriteInfo(laf.image),
				getHW()/Game::getScreen()->getWidthDensity(),
				getHH()*Game::getScreen()->getHeight()/2.0f));
	}
}

void Dialog::draw(){
	/*if(sprite){
		sprite->draw(getPosition());
	}else{*/
		Game::getPrimitiveDrawer()->drawBorderedFilledRectangle(
				getPosition(),
				getSize(),
				laf.borderThikness,
				laf.backColor,
				laf.borderColor);
//	}
	ElementCollection::draw();
}
