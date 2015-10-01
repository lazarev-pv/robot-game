#include <ChibiEngine/UserInterface/Menu.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include <ChibiEngine/Render/Sprites/NineSliceSprite.h>
#include <ChibiEngine/UserInterface/ContextMenu.h>
#include <ChibiEngine/Common/RAIIUtils.h>
#include <ChibiEngine/Clock/EventSystem.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Render/ScreenSystem.h>


using namespace game;
using namespace std;
using namespace glm;

const string Menu::TYPE_NAME = "Menu";

class Menu::Button : public UIElement {
public:
	Button(const std::string& name, int num, Menu* parent):
		laf(Game::getLookAndFeel().menu),
		hovered(false),
		num(num),
		parent(parent),
		text(laf.font, name){

        setSize(SpriteText::toScreenSize(vec2(
                text.getWorldHW()+2*(laf.padding.x+laf.padding.z),
                text.getWorldHH()+(laf.padding.y+laf.padding.w)
        )));

	}

	void executeLostFocus(){
		Game::getEventSystem()->doNextFrame(bind(&Menu::closeAll, parent), 1);
	}

	virtual void draw(){
		text.draw(getPosition(),hovered?Color::Yellow:laf.font.color);
       // Game::getPrimitiveDrawer()->drawRectangleBorder(createMatrix(getPosition(), getSize()), Color::White);
	};

	virtual void executeAction(){
		parent->openAt(num);
	}

private:
	const MenuLAF& laf;
	bool hovered;
	int num;
	Menu* parent;
	SpriteText text;
};


Menu::Element::Element(const std::string& name, const std::string& parentName, ContextMenuElemType* iter,int num, Menu* parent)
	:label(new Menu::Button(name,num,parent)),
	 subMenu(new ButtonContextMenu(iter,parentName+"."+name)){
}

Menu::Menu(MenuType* iter)
	:ElementCollection(),
	 openedNum(-1){

	float maxHH = 0;
	float sumHW = 0;

	setName(iter->name);
	int num=0;
	for(ContextMenuElemType* elem : iter->elem){
		Element el(elem->name,iter->name,elem,num,this);
		num++;
		subMenues.push_back(el);
		maxHH = std::max(el.label->getHH(),maxHH);
		sumHW += el.label->getHW();
		add(el.subMenu);
		add(el.label);
		el.subMenu->setVisible(false);
	}
	setPosition(vec3(-1.0+sumHW,1-maxHH,0));
	move(vec3(0, 0, -10));
	setSize(vec2(1,maxHH));

	if(subMenues.size()>0){
		subMenues[0].label->setPosition(vec3(-1+subMenues[0].label->getHW(),1-maxHH,1));
		subMenues[0].label->setSize(vec2(subMenues[0].label->getHW(),maxHH));
	}
	for(size_t i=1;i<subMenues.size();++i){
		subMenues[i].label->setPosition(vec3(subMenues[i-1].label->getPosition().x+subMenues[i-1].label->getHW()+subMenues[i].label->getHW(),1-maxHH,1));
		subMenues[i].label->setSize(vec2(subMenues[i].label->getHW(),maxHH));
	}

	const MenuLAF& laf = Game::getLookAndFeel().menu;
	back.reset(new NineSliceSprite(Game::getResources()->findSpriteInfo(laf.image),
			Game::getScreen()->getWidth(),maxHH/Game::getScreen()->getHeightDensityNoAspect()));

}

void Menu::draw(){
	back->draw(getPosition());
}

void Menu::closeAll(){
	for(auto& el : subMenues){
		el.subMenu->hideAll();
	}
}

void Menu::openAt(int i){
	closeAll();
	if(openedNum!=i){
		vec3 pos = subMenues[i].label->getPosition();
		pos.x -= subMenues[i].label->getHW();
		pos.y -= getHH() /*+ subMenues[i].subMenu->getAverageHeight()*/;
		subMenues[i].subMenu->viewAt(vec2(pos.x,pos.y));
	}
}
