#include <ChibiEngine/UserInterface/LookAndFeel.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/XMLParser/LafParser.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/Render/Sprites/NineSliceSprite.h>

using namespace game;
using namespace std;
using namespace glm;
using namespace rapidxml;

const static int DEFAUL_FONT_SIZE = 4;
const static float DEFAULT_BORDER_SIZE = 0.005;
const static float DEFAULT_CURSOR_HW = 0.0025;
const static string DEFAUL_FONT_NAME = "default";


Color game::getColor(ColorType* color, const LookAndFeel* parent){
	if(color==nullptr && parent!=nullptr){
        return parent->defaultColor;
	}

    return parse(color);
}

static inline FontLAF getFont(FontLAFType* data, Resources* res, const LookAndFeel* parent){
	if(data!=nullptr)
		return FontLAF(data,res, parent);
	else
		return parent->font;
}

static inline ButtonStateLAF getState(ButtonStateLAFType* data, Resources* res, const LookAndFeel* parent){
	if(data!=nullptr){
		return ButtonStateLAF(data,res,parent);
	}else{
		return parent->buttons.normal;
	}
}

static inline ButtonLAF getButton(ButtonLAFType* data, Resources* res, const LookAndFeel* parent){
	if(data!=nullptr){
		return ButtonLAF(data,res,parent);
	}else{
		return parent->buttons;
	}
}

static vec4 getPadding(PaddingType* data){
	if(data!=nullptr){
		return vec4(data->left,
				data->top,
				data->right,
				data->down);
	}else{
		return vec4(7,2,7,2);
	}
}

DialogCaptureLAF::DialogCaptureLAF(DialogCaptureLAFType* data, Resources* res, const LookAndFeel* parent)
	:height(data!=nullptr?data->height:-1),
	 controlWidth(data!=nullptr?data->controlsWidth:-1),
	 font(getFont(data!=nullptr?data->font:nullptr,res,parent)),
	 color(getColor(data!=nullptr?data->color:nullptr,parent)),
	 controlButtons(getButton(data!=nullptr?data->controlButtons:nullptr,res,parent)){}

DialogLAF::DialogLAF(DialogsLAFType* data, Resources* res, const LookAndFeel* parent)
	:capture(data!=nullptr?data->capture:nullptr,res, parent),
	 borderThikness(data!=nullptr && data->background!=nullptr?
			 data->background->borderThickness:DEFAULT_BORDER_SIZE),
	 backColor(getColor(data!=nullptr && data->background!=nullptr?
			 data->background->backColor:nullptr,
			 parent)),
	 borderColor(getColor(data!=nullptr && data->background!=nullptr?
			 data->background->borderColor:nullptr,
			 parent)),
	 image(data!=nullptr?data->image:""){
}

ButtonStateLAF::ButtonStateLAF(ButtonStateLAFType* data, Resources* res, const LookAndFeel* parent):
	font(getFont(data!=nullptr?data->font:nullptr,res,parent)),
	backColor(getColor(data!=nullptr?data->backColor:nullptr,parent)),
	borderColor(getColor(data!=nullptr?data->borderColor:nullptr,parent)),
	imageName(data!=nullptr?data->image:""){}

ButtonLAF::ButtonLAF(ButtonLAFType* data, Resources* res, const LookAndFeel* parent):
	normal(data!=nullptr?data->normal:nullptr,res,parent),
	pushed(data!=nullptr?getState(data->pushed,res,parent):getState(nullptr,res,parent)),
	hovered(data!=nullptr?getState(data->hovered,res,parent):getState(nullptr,res,parent)),
	borderThickness(data!=nullptr?data->borderThickness:DEFAULT_BORDER_SIZE),
	padding(data!=nullptr?getPadding(data->padding):getPadding(nullptr)),
	animatedImage(data!=nullptr?data->animatedImage:""){}

FontLAF::FontLAF(FontLAFType* data, Resources* res, const LookAndFeel* parent):
	font(res->findFont(data!=nullptr && data->face.size()>0?data->face:DEFAUL_FONT_NAME)),
	size(data!=nullptr && data->size>0?data->size:DEFAUL_FONT_SIZE),
	color(getColor(data!=nullptr?data->textColor:nullptr, parent)){}


LookAndFeel LookAndFeel::create(const std::string& fileName){
    auto root(createRoot<LafParserRoot>(fileName));

    Resources* r = Game::getResources();
    r->load(root.lookAndFeel->resources, fileName, true);
    return LookAndFeel(root.lookAndFeel,r);
}


LabelsListLAF::LabelsListLAF(LabelsListLAFType *iter, const LookAndFeel* parent):
    normalTextColor(getColor(iter!=nullptr?iter->normalTextColor:nullptr, parent)),
    selectedTextColor(getColor(iter!=nullptr?iter->selectedTextColor:nullptr, parent)),
    selectionColor(getColor(iter!=nullptr?iter->selectionColor:nullptr, parent)),
    backgroundColor(getColor(iter!=nullptr?iter->backgroundColor:nullptr, parent)),
    borderColor(getColor(iter!=nullptr?iter->borderColor:nullptr, parent)),
    borderWidth(iter!=nullptr?
            vec2(iter->borderWidth*Game::getScreen()->getWidthDensity(),
                    iter->borderWidth*Game::getScreen()->getHeightDensityNoAspect()):
            vec2(0,0)),
    scroller(iter!=nullptr?iter->scroller:nullptr,parent)
{
        normalTextColor = getColor(iter != nullptr ? iter->normalTextColor : nullptr, parent);
        selectedTextColor = getColor(iter != nullptr ? iter->selectedTextColor : nullptr, parent);
    {
        vec4 p = getPadding(iter!=nullptr?iter->padding:nullptr);
        padding.x = (p.x+p.z)*0.5*Game::getScreen()->getWidthDensity();
        padding.y = (p.y+p.w)*0.5*Game::getScreen()->getHeightDensityNoAspect();
    }
}

ScrollerLAF::ScrollerLAF(ScrollerLAFType *iter, const LookAndFeel* parent):
    backColor(getColor(iter!=nullptr?iter->backColor:nullptr, parent)),
    borderColor(getColor(iter!=nullptr?iter->borderColor:nullptr, parent)),
    image(iter!=nullptr?iter->image:""),
    hoveredImage(iter!=nullptr?iter->hoveredImage:""),
    worldWidth(iter!=nullptr?iter->width:0),
    worldHeight(iter!=nullptr?iter->height:0),
    borderWidth(iter!=nullptr?
            vec2(iter->borderWidth*Game::getScreen()->getWidthDensity(),
                 iter->borderWidth*Game::getScreen()->getHeightDensityNoAspect()):
            vec2(0,0)) {
}

LookAndFeel::LookAndFeel(LookAndFeelType* data, Resources* res):
	defaultColor(getColor(data!=nullptr?data->defaultColor:nullptr,nullptr)),
	defaultPadding(getPadding(nullptr)),
	font(data!=nullptr?data->font:nullptr,res,this),
	buttons(data!=nullptr?data->buttons:nullptr,res,this),
	dialogs(data!=nullptr?data->dialogs:nullptr,res,this),
	menu(data!=nullptr?data->menu:nullptr,res,this),
	inputField(data!=nullptr?data->inputField:nullptr,res,this),
	slider(data!=nullptr?data->slider:nullptr,res,this),
	checkbox(data!=nullptr?data->checkbox:nullptr,res),
	rgb(data!=nullptr?data->rgbSlider:nullptr),
    labelsList(data!=nullptr?data->labelsList:nullptr, this),
    imagePicker(data!=nullptr?data->imagePicker:nullptr,this),
	healthbar(data!=nullptr?data->healthbar:nullptr, this),
	caption(data!=nullptr?data->caption:nullptr, this) {
}

RGBSliderLAF::RGBSliderLAF(RGBSliderLAFType* data)
	:colorHW(data!=nullptr?data->colorHW:0.0f),
	 hh(data!=nullptr?data->hh:0.0f){
}

MenuLAF::MenuLAF(MenuLAFType* data, Resources* res, const LookAndFeel* parent):
	backColor(getColor(data!=nullptr?data->normalColor:nullptr,nullptr)),
	borderColor(getColor(data!=nullptr?data->borderColor:nullptr,nullptr)),
	hoveredColor(getColor(data!=nullptr?data->hoveredColor:nullptr,nullptr)),
	font(getFont(data!=nullptr?data->font:nullptr,res,parent)),
	borderThickness(data!=nullptr?data->borderThickness:DEFAULT_BORDER_SIZE),
	image(data!=nullptr?data->image:""),
	hoverImage(data!=nullptr?data->hoverImage:""),
	padding(getPadding(data!=nullptr?data->padding:nullptr)){}

InputFieldLAF::InputFieldLAF(InputFieldLAFType* data, Resources* res, const LookAndFeel* parent):
	backColor(getColor(data!=nullptr?data->backColor:nullptr,nullptr)),
	disBackColor(getColor(data!=nullptr?data->disBackColor:nullptr,nullptr)),
	selectColor(getColor(data!=nullptr?data->selectColor:nullptr,nullptr)),
	selectedBorderColor(getColor(data!=nullptr?data->selectedBorderColor:nullptr,nullptr)),
	borderColor(getColor(data!=nullptr?data->borderColor:nullptr,nullptr)),
	padding(getPadding(data!=nullptr?data->padding:nullptr)),
	borderThickness(data!=nullptr?data->borderThickness:DEFAULT_BORDER_SIZE),
	image(data!=nullptr?data->image:""),
	normal(data!=nullptr?data->normal:""),
	focused(data!=nullptr?data->focused:""),
	font(getFont(data!=nullptr?data->font:nullptr,res,parent)),
	cursorHW(data!=nullptr?data->cursorHW:DEFAULT_CURSOR_HW),
	cursorColor(getColor(data!=nullptr?data->cursorColor:nullptr,nullptr))
{}

CheckboxLAF::CheckboxLAF(CheckboxLAFType* data,Resources* res)
	:hw(data!=nullptr?data->hw:0),
	hh(data!=nullptr?data->hw:0),
	normal{},checked{},
	normalScale(0,0),checkedScale(0,0){

	hw*=Game::getScreen()->getWidthDensity();
	hh*=Game::getScreen()->getHeightDensityNoAspect();

	if(data!=nullptr){
		normal=res->findSprite(data->normal_image);
		checked=res->findSprite(data->checked_image);
	}
	if(normal!=nullptr){
		normalScale.x = hw/normal->getHW();
		normalScale.y = hh/normal->getHW();
	}
	if(checked!=nullptr){
		checkedScale.x = hw/checked->getHW();
		checkedScale.y = hh/checked->getHW();
	}
}

SliderLAF::SliderLAF(SliderLAFType* data,Resources* , const LookAndFeel* parent):
recColor(getColor(data!=nullptr?data->recColor:nullptr,parent)),
hoveredColor(getColor(data!=nullptr?data->hoveredColor:nullptr,parent)),
markColor(getColor(data!=nullptr?data->markColor:nullptr,parent)),
defaultPercision(data!=nullptr?data->defaultPercision:0){
    textColor= getColor(data!=nullptr?data->texColor:nullptr,parent);

	if(data!=nullptr){
		markLenX=Game::getScreen()->getWidthDensity()*data->markLen;
		markLenY=Game::getScreen()->getHeightDensityNoAspect()*data->markLen;
		hh=Game::getScreen()->getHeightDensityNoAspect()*data->hh;
		baseHH=Game::getScreen()->getHeightDensityNoAspect()*data->baseHH;
	}else{
		markLenX=0;
		markLenY=0;
		hh=0;
		baseHH=0;
	}
}


ImagePickerLAF::ImagePickerLAF(ImagePickerLAFType *data, const LookAndFeel *parent):
    labelsOffset(parse(data!=nullptr?data->labelsOffset:nullptr)),
    labelsSize(parse(data!=nullptr?data->labelsSize:nullptr)),
    imageBorderColor(getColor(data!=nullptr?data->imageBorderColor:nullptr,parent)),
    imageOffset(parse(data!=nullptr?data->imageOffset:nullptr)),
    imageSize(parse(data!=nullptr?data->imageSize:nullptr)),
    baseSize(parse(data!=nullptr?data->baseSize:nullptr)){
}

HealthbarLAF::HealthbarLAF(HealthbarLAFType *data, const LookAndFeel* )
	:hh(data!=nullptr?data->hh:10),
	 defaultPercision(data!=nullptr?data->defaultPercision:0),
	 image(data!=nullptr?Game::getResources()->findSprite(data->image):nullptr),
	 backImage(Game::getResources()->findSpriteInfo(data->backImage)),
	 padding(data!=nullptr?data->borderThickness:0,data!=nullptr?data->borderThickness:0){

	ScreenSystem* screen = Game::getScreen();
	padding.x*=screen->getWidthDensity();
	padding.y*=screen->getHeightDensityNoAspect();

	hh*=screen->getHeightDensityNoAspect();
}

CaptionLAF::CaptionLAF(CaptionLAFType* iter,const LookAndFeel* parent)
    :hh(iter!=nullptr?iter->hh:10),
     color(getColor(iter!=nullptr?iter->color:nullptr,parent)),
     textColor(getColor(iter!=nullptr?iter->textColor:nullptr,parent))
{

    ScreenSystem* screen = Game::getScreen();
    hh*=screen->getHeightDensityNoAspect();
}