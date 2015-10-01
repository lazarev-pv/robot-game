#include <ChibiEngine/XMLParser/LafParser.h>
#include <cstring>
#include "util.h"
using namespace rapidxml;
using namespace std;
ButtonLAFType::ButtonLAFType()
	:normal(0x0),hovered(0x0),pushed(0x0),padding(0x0),animatedImage(""){}

ButtonLAFType::ButtonLAFType(ButtonLAFType&& other)
	:normal(other.normal),hovered(other.hovered),pushed(other.pushed),padding(other.padding),borderThickness(other.borderThickness),animatedImage(other.animatedImage)
{
	other.normal=nullptr;
	other.hovered=nullptr;
	other.pushed=nullptr;
	other.padding=nullptr;
}
ButtonLAFType::ButtonLAFType(rapidxml::xml_node<>*  node)
	:normal(0x0),hovered(0x0),pushed(0x0),padding(0x0),animatedImage(""){
	setValue(&borderThickness, node->first_attribute("borderThickness"));
	setValue(&animatedImage, node->first_attribute("animatedImage"));
		create(&normal, node->first_node("normal"));
		create(&hovered, node->first_node("hovered"));
		create(&pushed, node->first_node("pushed"));
		create(&padding, node->first_node("padding"));
}

ButtonLAFType::~ButtonLAFType(){
	delete normal;
	delete hovered;
	delete pushed;
	delete padding;
}

void ButtonLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" borderThickness=\""<<borderThickness<<"\"";
		out<<" animatedImage=\""<<animatedImage<<"\"";
		out<<">\n";
	}
	if(normal!=0x0){
		normal->save("normal",out,nestingLevel+1);
	}
	if(hovered!=0x0){
		hovered->save("hovered",out,nestingLevel+1);
	}
	if(pushed!=0x0){
		pushed->save("pushed",out,nestingLevel+1);
	}
	if(padding!=0x0){
		padding->save("padding",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ButtonStateLAFType::ButtonStateLAFType()
	:font(0x0),backColor(0x0),borderColor(0x0){}

ButtonStateLAFType::ButtonStateLAFType(ButtonStateLAFType&& other)
	:font(other.font),backColor(other.backColor),borderColor(other.borderColor),image(other.image)
{
	other.font=nullptr;
	other.backColor=nullptr;
	other.borderColor=nullptr;
}
ButtonStateLAFType::ButtonStateLAFType(rapidxml::xml_node<>*  node)
	:font(0x0),backColor(0x0),borderColor(0x0){
		create(&font, node->first_node("font"));
		create(&backColor, node->first_node("backColor"));
		create(&borderColor, node->first_node("borderColor"));
		setValue(&image, node->first_node("image"));
}

ButtonStateLAFType::~ButtonStateLAFType(){
	delete font;
	delete backColor;
	delete borderColor;
}

void ButtonStateLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(font!=0x0){
		font->save("font",out,nestingLevel+1);
	}
	if(backColor!=0x0){
		backColor->save("backColor",out,nestingLevel+1);
	}
	if(borderColor!=0x0){
		borderColor->save("borderColor",out,nestingLevel+1);
	}
		out<<nestPrefix;
		createTag(out,"image",image);
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

CaptionLAFType::CaptionLAFType()
	:color(0x0),textColor(0x0){}

CaptionLAFType::CaptionLAFType(CaptionLAFType&& other)
	:color(other.color),textColor(other.textColor),hh(other.hh)
{
	other.color=nullptr;
	other.textColor=nullptr;
}
CaptionLAFType::CaptionLAFType(rapidxml::xml_node<>*  node)
	:color(0x0),textColor(0x0){
	setValue(&hh, node->first_attribute("hh"));
		create(&color, node->first_node("color"));
		create(&textColor, node->first_node("textColor"));
}

CaptionLAFType::~CaptionLAFType(){
	delete color;
	delete textColor;
}

void CaptionLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" hh=\""<<hh<<"\"";
		out<<">\n";
	}
	if(color!=0x0){
		color->save("color",out,nestingLevel+1);
	}
	if(textColor!=0x0){
		textColor->save("textColor",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

CheckboxLAFType::CheckboxLAFType()
	:normal_image(""),checked_image(""){}

CheckboxLAFType::CheckboxLAFType(CheckboxLAFType&& other)
	:normal_image(other.normal_image),checked_image(other.checked_image),hw(other.hw),hh(other.hh)
{
}
CheckboxLAFType::CheckboxLAFType(rapidxml::xml_node<>*  node)
	:normal_image(""),checked_image(""){
	setValue(&normal_image, node->first_attribute("normal_image"));
	setValue(&checked_image, node->first_attribute("checked_image"));
	setValue(&hw, node->first_attribute("hw"));
	setValue(&hh, node->first_attribute("hh"));
}

CheckboxLAFType::~CheckboxLAFType(){
}

void CheckboxLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" normal_image=\""<<normal_image<<"\"";
		out<<" checked_image=\""<<checked_image<<"\"";
		out<<" hw=\""<<hw<<"\"";
		out<<" hh=\""<<hh<<"\"";
		out<<"/>";
	}
	out<<endl;
}

DialogBackgroundLAFType::DialogBackgroundLAFType()
	:backColor(0x0),borderColor(0x0),borderThickness(0){}

DialogBackgroundLAFType::DialogBackgroundLAFType(DialogBackgroundLAFType&& other)
	:backColor(other.backColor),borderColor(other.borderColor),borderThickness(other.borderThickness)
{
	other.backColor=nullptr;
	other.borderColor=nullptr;
}
DialogBackgroundLAFType::DialogBackgroundLAFType(rapidxml::xml_node<>*  node)
	:backColor(0x0),borderColor(0x0),borderThickness(0){
	setValue(&borderThickness, node->first_attribute("borderThickness"));
		create(&backColor, node->first_node("backColor"));
		create(&borderColor, node->first_node("borderColor"));
}

DialogBackgroundLAFType::~DialogBackgroundLAFType(){
	delete backColor;
	delete borderColor;
}

void DialogBackgroundLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" borderThickness=\""<<borderThickness<<"\"";
		out<<">\n";
	}
	if(backColor!=0x0){
		backColor->save("backColor",out,nestingLevel+1);
	}
	if(borderColor!=0x0){
		borderColor->save("borderColor",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

DialogCaptureLAFType::DialogCaptureLAFType()
	:font(0x0),color(0x0),controlButtons(0x0),height(-1),controlsWidth(-1){}

DialogCaptureLAFType::DialogCaptureLAFType(DialogCaptureLAFType&& other)
	:font(other.font),color(other.color),controlButtons(other.controlButtons),height(other.height),controlsWidth(other.controlsWidth)
{
	other.font=nullptr;
	other.color=nullptr;
	other.controlButtons=nullptr;
}
DialogCaptureLAFType::DialogCaptureLAFType(rapidxml::xml_node<>*  node)
	:font(0x0),color(0x0),controlButtons(0x0),height(-1),controlsWidth(-1){
	setValue(&height, node->first_attribute("height"));
	setValue(&controlsWidth, node->first_attribute("controlsWidth"));
		create(&font, node->first_node("font"));
		create(&color, node->first_node("color"));
		create(&controlButtons, node->first_node("controlButtons"));
}

DialogCaptureLAFType::~DialogCaptureLAFType(){
	delete font;
	delete color;
	delete controlButtons;
}

void DialogCaptureLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" height=\""<<height<<"\"";
		out<<" controlsWidth=\""<<controlsWidth<<"\"";
		out<<">\n";
	}
	if(font!=0x0){
		font->save("font",out,nestingLevel+1);
	}
	if(color!=0x0){
		color->save("color",out,nestingLevel+1);
	}
	if(controlButtons!=0x0){
		controlButtons->save("controlButtons",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

DialogsLAFType::DialogsLAFType()
	:capture(0x0),background(0x0),image(""){}

DialogsLAFType::DialogsLAFType(DialogsLAFType&& other)
	:capture(other.capture),background(other.background),image(other.image)
{
	other.capture=nullptr;
	other.background=nullptr;
}
DialogsLAFType::DialogsLAFType(rapidxml::xml_node<>*  node)
	:capture(0x0),background(0x0),image(""){
	setValue(&image, node->first_attribute("image"));
		create(&capture, node->first_node("capture"));
		create(&background, node->first_node("background"));
}

DialogsLAFType::~DialogsLAFType(){
	delete capture;
	delete background;
}

void DialogsLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" image=\""<<image<<"\"";
		out<<">\n";
	}
	if(capture!=0x0){
		capture->save("capture",out,nestingLevel+1);
	}
	if(background!=0x0){
		background->save("background",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

FontLAFType::FontLAFType()
	:textColor(0x0),face(""){}

FontLAFType::FontLAFType(FontLAFType&& other)
	:textColor(other.textColor),size(other.size),face(other.face)
{
	other.textColor=nullptr;
}
FontLAFType::FontLAFType(rapidxml::xml_node<>*  node)
	:textColor(0x0),face(""){
	setValue(&size, node->first_attribute("size"));
	setValue(&face, node->first_attribute("face"));
		create(&textColor, node->first_node("textColor"));
}

FontLAFType::~FontLAFType(){
	delete textColor;
}

void FontLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" size=\""<<size<<"\"";
		out<<" face=\""<<face<<"\"";
		out<<">\n";
	}
	if(textColor!=0x0){
		textColor->save("textColor",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

HealthbarLAFType::HealthbarLAFType()
	:defaultPercision(0),image(""),backImage(""){}

HealthbarLAFType::HealthbarLAFType(HealthbarLAFType&& other)
	:hh(other.hh),defaultPercision(other.defaultPercision),image(other.image),backImage(other.backImage),borderThickness(other.borderThickness)
{
}
HealthbarLAFType::HealthbarLAFType(rapidxml::xml_node<>*  node)
	:defaultPercision(0),image(""),backImage(""){
	setValue(&hh, node->first_attribute("hh"));
	setValue(&defaultPercision, node->first_attribute("defaultPercision"));
	setValue(&image, node->first_attribute("image"));
	setValue(&backImage, node->first_attribute("backImage"));
	setValue(&borderThickness, node->first_attribute("borderThickness"));
}

HealthbarLAFType::~HealthbarLAFType(){
}

void HealthbarLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" hh=\""<<hh<<"\"";
		out<<" defaultPercision=\""<<defaultPercision<<"\"";
		out<<" image=\""<<image<<"\"";
		out<<" backImage=\""<<backImage<<"\"";
		out<<" borderThickness=\""<<borderThickness<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ImagePickerLAFType::ImagePickerLAFType()
	:labelsOffset(0x0),labelsSize(0x0),imageBorderColor(0x0),imageOffset(0x0),imageSize(0x0),baseSize(0x0){}

ImagePickerLAFType::ImagePickerLAFType(ImagePickerLAFType&& other)
	:labelsOffset(other.labelsOffset),labelsSize(other.labelsSize),imageBorderColor(other.imageBorderColor),imageOffset(other.imageOffset),imageSize(other.imageSize),baseSize(other.baseSize)
{
	other.labelsOffset=nullptr;
	other.labelsSize=nullptr;
	other.imageBorderColor=nullptr;
	other.imageOffset=nullptr;
	other.imageSize=nullptr;
	other.baseSize=nullptr;
}
ImagePickerLAFType::ImagePickerLAFType(rapidxml::xml_node<>*  node)
	:labelsOffset(0x0),labelsSize(0x0),imageBorderColor(0x0),imageOffset(0x0),imageSize(0x0),baseSize(0x0){
		create(&labelsOffset, node->first_node("labelsOffset"));
		create(&labelsSize, node->first_node("labelsSize"));
		create(&imageBorderColor, node->first_node("imageBorderColor"));
		create(&imageOffset, node->first_node("imageOffset"));
		create(&imageSize, node->first_node("imageSize"));
		create(&baseSize, node->first_node("baseSize"));
}

ImagePickerLAFType::~ImagePickerLAFType(){
	delete labelsOffset;
	delete labelsSize;
	delete imageBorderColor;
	delete imageOffset;
	delete imageSize;
	delete baseSize;
}

void ImagePickerLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(labelsOffset!=0x0){
		labelsOffset->save("labelsOffset",out,nestingLevel+1);
	}
	if(labelsSize!=0x0){
		labelsSize->save("labelsSize",out,nestingLevel+1);
	}
	if(imageBorderColor!=0x0){
		imageBorderColor->save("imageBorderColor",out,nestingLevel+1);
	}
	if(imageOffset!=0x0){
		imageOffset->save("imageOffset",out,nestingLevel+1);
	}
	if(imageSize!=0x0){
		imageSize->save("imageSize",out,nestingLevel+1);
	}
	if(baseSize!=0x0){
		baseSize->save("baseSize",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

InputFieldLAFType::InputFieldLAFType()
	:font(0x0),backColor(0x0),disBackColor(0x0),selectColor(0x0),borderColor(0x0),selectedBorderColor(0x0),cursorColor(0x0),padding(0x0),image(""),normal(""),focused(""){}

InputFieldLAFType::InputFieldLAFType(InputFieldLAFType&& other)
	:font(other.font),backColor(other.backColor),disBackColor(other.disBackColor),selectColor(other.selectColor),borderColor(other.borderColor),selectedBorderColor(other.selectedBorderColor),cursorColor(other.cursorColor),padding(other.padding),borderThickness(other.borderThickness),cursorHW(other.cursorHW),image(other.image),normal(other.normal),focused(other.focused)
{
	other.font=nullptr;
	other.backColor=nullptr;
	other.disBackColor=nullptr;
	other.selectColor=nullptr;
	other.borderColor=nullptr;
	other.selectedBorderColor=nullptr;
	other.cursorColor=nullptr;
	other.padding=nullptr;
}
InputFieldLAFType::InputFieldLAFType(rapidxml::xml_node<>*  node)
	:font(0x0),backColor(0x0),disBackColor(0x0),selectColor(0x0),borderColor(0x0),selectedBorderColor(0x0),cursorColor(0x0),padding(0x0),image(""),normal(""),focused(""){
	setValue(&borderThickness, node->first_attribute("borderThickness"));
	setValue(&cursorHW, node->first_attribute("cursorHW"));
	setValue(&image, node->first_attribute("image"));
	setValue(&normal, node->first_attribute("normal"));
	setValue(&focused, node->first_attribute("focused"));
		create(&font, node->first_node("font"));
		create(&backColor, node->first_node("backColor"));
		create(&disBackColor, node->first_node("disBackColor"));
		create(&selectColor, node->first_node("selectColor"));
		create(&borderColor, node->first_node("borderColor"));
		create(&selectedBorderColor, node->first_node("selectedBorderColor"));
		create(&cursorColor, node->first_node("cursorColor"));
		create(&padding, node->first_node("padding"));
}

InputFieldLAFType::~InputFieldLAFType(){
	delete font;
	delete backColor;
	delete disBackColor;
	delete selectColor;
	delete borderColor;
	delete selectedBorderColor;
	delete cursorColor;
	delete padding;
}

void InputFieldLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" borderThickness=\""<<borderThickness<<"\"";
		out<<" cursorHW=\""<<cursorHW<<"\"";
		out<<" image=\""<<image<<"\"";
		out<<" normal=\""<<normal<<"\"";
		out<<" focused=\""<<focused<<"\"";
		out<<">\n";
	}
	if(font!=0x0){
		font->save("font",out,nestingLevel+1);
	}
	if(backColor!=0x0){
		backColor->save("backColor",out,nestingLevel+1);
	}
	if(disBackColor!=0x0){
		disBackColor->save("disBackColor",out,nestingLevel+1);
	}
	if(selectColor!=0x0){
		selectColor->save("selectColor",out,nestingLevel+1);
	}
	if(borderColor!=0x0){
		borderColor->save("borderColor",out,nestingLevel+1);
	}
	if(selectedBorderColor!=0x0){
		selectedBorderColor->save("selectedBorderColor",out,nestingLevel+1);
	}
	if(cursorColor!=0x0){
		cursorColor->save("cursorColor",out,nestingLevel+1);
	}
	if(padding!=0x0){
		padding->save("padding",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

LabelsListLAFType::LabelsListLAFType()
	:padding(0x0),normalTextColor(0x0),selectedTextColor(0x0),selectionColor(0x0),backgroundColor(0x0),borderColor(0x0),scroller(0x0){}

LabelsListLAFType::LabelsListLAFType(LabelsListLAFType&& other)
	:padding(other.padding),normalTextColor(other.normalTextColor),selectedTextColor(other.selectedTextColor),selectionColor(other.selectionColor),backgroundColor(other.backgroundColor),borderColor(other.borderColor),scroller(other.scroller),borderWidth(other.borderWidth)
{
	other.padding=nullptr;
	other.normalTextColor=nullptr;
	other.selectedTextColor=nullptr;
	other.selectionColor=nullptr;
	other.backgroundColor=nullptr;
	other.borderColor=nullptr;
	other.scroller=nullptr;
}
LabelsListLAFType::LabelsListLAFType(rapidxml::xml_node<>*  node)
	:padding(0x0),normalTextColor(0x0),selectedTextColor(0x0),selectionColor(0x0),backgroundColor(0x0),borderColor(0x0),scroller(0x0){
	setValue(&borderWidth, node->first_attribute("borderWidth"));
		create(&padding, node->first_node("padding"));
		create(&normalTextColor, node->first_node("normalTextColor"));
		create(&selectedTextColor, node->first_node("selectedTextColor"));
		create(&selectionColor, node->first_node("selectionColor"));
		create(&backgroundColor, node->first_node("backgroundColor"));
		create(&borderColor, node->first_node("borderColor"));
		create(&scroller, node->first_node("scroller"));
}

LabelsListLAFType::~LabelsListLAFType(){
	delete padding;
	delete normalTextColor;
	delete selectedTextColor;
	delete selectionColor;
	delete backgroundColor;
	delete borderColor;
	delete scroller;
}

void LabelsListLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" borderWidth=\""<<borderWidth<<"\"";
		out<<">\n";
	}
	if(padding!=0x0){
		padding->save("padding",out,nestingLevel+1);
	}
	if(normalTextColor!=0x0){
		normalTextColor->save("normalTextColor",out,nestingLevel+1);
	}
	if(selectedTextColor!=0x0){
		selectedTextColor->save("selectedTextColor",out,nestingLevel+1);
	}
	if(selectionColor!=0x0){
		selectionColor->save("selectionColor",out,nestingLevel+1);
	}
	if(backgroundColor!=0x0){
		backgroundColor->save("backgroundColor",out,nestingLevel+1);
	}
	if(borderColor!=0x0){
		borderColor->save("borderColor",out,nestingLevel+1);
	}
	if(scroller!=0x0){
		scroller->save("scroller",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

LafParserRoot::LafParserRoot()
	:lookAndFeel(0x0){}

LafParserRoot::LafParserRoot(LafParserRoot&& other)
	:lookAndFeel(other.lookAndFeel)
{
	other.lookAndFeel=nullptr;
}
LafParserRoot::LafParserRoot(rapidxml::xml_node<>*  node)
	:lookAndFeel(0x0){
		create(&lookAndFeel, node->first_node("lookAndFeel"));
}

LafParserRoot::~LafParserRoot(){
	delete lookAndFeel;
}

void LafParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(lookAndFeel!=0x0){
		lookAndFeel->save("lookAndFeel",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

LookAndFeelType::LookAndFeelType()
	:resources(0x0),defaultColor(0x0),font(0x0),buttons(0x0),dialogs(0x0),menu(0x0),inputField(0x0),checkbox(0x0),slider(0x0),rgbSlider(0x0),labelsList(0x0),imagePicker(0x0),healthbar(0x0),caption(0x0){}

LookAndFeelType::LookAndFeelType(LookAndFeelType&& other)
	:resources(other.resources),defaultColor(other.defaultColor),font(other.font),buttons(other.buttons),dialogs(other.dialogs),menu(other.menu),inputField(other.inputField),checkbox(other.checkbox),slider(other.slider),rgbSlider(other.rgbSlider),labelsList(other.labelsList),imagePicker(other.imagePicker),healthbar(other.healthbar),caption(other.caption)
{
	other.resources=nullptr;
	other.defaultColor=nullptr;
	other.font=nullptr;
	other.buttons=nullptr;
	other.dialogs=nullptr;
	other.menu=nullptr;
	other.inputField=nullptr;
	other.checkbox=nullptr;
	other.slider=nullptr;
	other.rgbSlider=nullptr;
	other.labelsList=nullptr;
	other.imagePicker=nullptr;
	other.healthbar=nullptr;
	other.caption=nullptr;
}
LookAndFeelType::LookAndFeelType(rapidxml::xml_node<>*  node)
	:resources(0x0),defaultColor(0x0),font(0x0),buttons(0x0),dialogs(0x0),menu(0x0),inputField(0x0),checkbox(0x0),slider(0x0),rgbSlider(0x0),labelsList(0x0),imagePicker(0x0),healthbar(0x0),caption(0x0){
		create(&resources, node->first_node("resources"));
		create(&defaultColor, node->first_node("defaultColor"));
		create(&font, node->first_node("font"));
		create(&buttons, node->first_node("buttons"));
		create(&dialogs, node->first_node("dialogs"));
		create(&menu, node->first_node("menu"));
		create(&inputField, node->first_node("inputField"));
		create(&checkbox, node->first_node("checkbox"));
		create(&slider, node->first_node("slider"));
		create(&rgbSlider, node->first_node("rgbSlider"));
		create(&labelsList, node->first_node("labelsList"));
		create(&imagePicker, node->first_node("imagePicker"));
		create(&healthbar, node->first_node("healthbar"));
		create(&caption, node->first_node("caption"));
}

LookAndFeelType::~LookAndFeelType(){
	delete resources;
	delete defaultColor;
	delete font;
	delete buttons;
	delete dialogs;
	delete menu;
	delete inputField;
	delete checkbox;
	delete slider;
	delete rgbSlider;
	delete labelsList;
	delete imagePicker;
	delete healthbar;
	delete caption;
}

void LookAndFeelType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(resources!=0x0){
		resources->save("resources",out,nestingLevel+1);
	}
	if(defaultColor!=0x0){
		defaultColor->save("defaultColor",out,nestingLevel+1);
	}
	if(font!=0x0){
		font->save("font",out,nestingLevel+1);
	}
	if(buttons!=0x0){
		buttons->save("buttons",out,nestingLevel+1);
	}
	if(dialogs!=0x0){
		dialogs->save("dialogs",out,nestingLevel+1);
	}
	if(menu!=0x0){
		menu->save("menu",out,nestingLevel+1);
	}
	if(inputField!=0x0){
		inputField->save("inputField",out,nestingLevel+1);
	}
	if(checkbox!=0x0){
		checkbox->save("checkbox",out,nestingLevel+1);
	}
	if(slider!=0x0){
		slider->save("slider",out,nestingLevel+1);
	}
	if(rgbSlider!=0x0){
		rgbSlider->save("rgbSlider",out,nestingLevel+1);
	}
	if(labelsList!=0x0){
		labelsList->save("labelsList",out,nestingLevel+1);
	}
	if(imagePicker!=0x0){
		imagePicker->save("imagePicker",out,nestingLevel+1);
	}
	if(healthbar!=0x0){
		healthbar->save("healthbar",out,nestingLevel+1);
	}
	if(caption!=0x0){
		caption->save("caption",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

MenuLAFType::MenuLAFType()
	:font(0x0),normalColor(0x0),hoveredColor(0x0),borderColor(0x0),padding(0x0),image(""),hoverImage(""){}

MenuLAFType::MenuLAFType(MenuLAFType&& other)
	:font(other.font),normalColor(other.normalColor),hoveredColor(other.hoveredColor),borderColor(other.borderColor),padding(other.padding),borderThickness(other.borderThickness),image(other.image),hoverImage(other.hoverImage)
{
	other.font=nullptr;
	other.normalColor=nullptr;
	other.hoveredColor=nullptr;
	other.borderColor=nullptr;
	other.padding=nullptr;
}
MenuLAFType::MenuLAFType(rapidxml::xml_node<>*  node)
	:font(0x0),normalColor(0x0),hoveredColor(0x0),borderColor(0x0),padding(0x0),image(""),hoverImage(""){
	setValue(&borderThickness, node->first_attribute("borderThickness"));
	setValue(&image, node->first_attribute("image"));
	setValue(&hoverImage, node->first_attribute("hoverImage"));
		create(&font, node->first_node("font"));
		create(&normalColor, node->first_node("normalColor"));
		create(&hoveredColor, node->first_node("hoveredColor"));
		create(&borderColor, node->first_node("borderColor"));
		create(&padding, node->first_node("padding"));
}

MenuLAFType::~MenuLAFType(){
	delete font;
	delete normalColor;
	delete hoveredColor;
	delete borderColor;
	delete padding;
}

void MenuLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" borderThickness=\""<<borderThickness<<"\"";
		out<<" image=\""<<image<<"\"";
		out<<" hoverImage=\""<<hoverImage<<"\"";
		out<<">\n";
	}
	if(font!=0x0){
		font->save("font",out,nestingLevel+1);
	}
	if(normalColor!=0x0){
		normalColor->save("normalColor",out,nestingLevel+1);
	}
	if(hoveredColor!=0x0){
		hoveredColor->save("hoveredColor",out,nestingLevel+1);
	}
	if(borderColor!=0x0){
		borderColor->save("borderColor",out,nestingLevel+1);
	}
	if(padding!=0x0){
		padding->save("padding",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

PaddingType::PaddingType()
	:left(0.0),top(0.0),right(0.0),down(0.0){}

PaddingType::PaddingType(PaddingType&& other)
	:left(other.left),top(other.top),right(other.right),down(other.down)
{
}
PaddingType::PaddingType(rapidxml::xml_node<>*  node)
	:left(0.0),top(0.0),right(0.0),down(0.0){
	setValue(&left, node->first_attribute("left"));
	setValue(&top, node->first_attribute("top"));
	setValue(&right, node->first_attribute("right"));
	setValue(&down, node->first_attribute("down"));
}

PaddingType::~PaddingType(){
}

void PaddingType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" left=\""<<left<<"\"";
		out<<" top=\""<<top<<"\"";
		out<<" right=\""<<right<<"\"";
		out<<" down=\""<<down<<"\"";
		out<<"/>";
	}
	out<<endl;
}

RGBSliderLAFType::RGBSliderLAFType(){}

RGBSliderLAFType::RGBSliderLAFType(RGBSliderLAFType&& other)
	:hh(other.hh),colorHW(other.colorHW)
{
}
RGBSliderLAFType::RGBSliderLAFType(rapidxml::xml_node<>*  node){
	setValue(&hh, node->first_attribute("hh"));
	setValue(&colorHW, node->first_attribute("colorHW"));
}

RGBSliderLAFType::~RGBSliderLAFType(){
}

void RGBSliderLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" hh=\""<<hh<<"\"";
		out<<" colorHW=\""<<colorHW<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ScrollerLAFType::ScrollerLAFType()
	:backColor(0x0),borderColor(0x0),hoveredImage(""),image(""){}

ScrollerLAFType::ScrollerLAFType(ScrollerLAFType&& other)
	:backColor(other.backColor),borderColor(other.borderColor),width(other.width),height(other.height),borderWidth(other.borderWidth),hoveredImage(other.hoveredImage),image(other.image)
{
	other.backColor=nullptr;
	other.borderColor=nullptr;
}
ScrollerLAFType::ScrollerLAFType(rapidxml::xml_node<>*  node)
	:backColor(0x0),borderColor(0x0),hoveredImage(""),image(""){
	setValue(&width, node->first_attribute("width"));
	setValue(&height, node->first_attribute("height"));
	setValue(&borderWidth, node->first_attribute("borderWidth"));
	setValue(&hoveredImage, node->first_attribute("hoveredImage"));
	setValue(&image, node->first_attribute("image"));
		create(&backColor, node->first_node("backColor"));
		create(&borderColor, node->first_node("borderColor"));
}

ScrollerLAFType::~ScrollerLAFType(){
	delete backColor;
	delete borderColor;
}

void ScrollerLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" width=\""<<width<<"\"";
		out<<" height=\""<<height<<"\"";
		out<<" borderWidth=\""<<borderWidth<<"\"";
		out<<" hoveredImage=\""<<hoveredImage<<"\"";
		out<<" image=\""<<image<<"\"";
		out<<">\n";
	}
	if(backColor!=0x0){
		backColor->save("backColor",out,nestingLevel+1);
	}
	if(borderColor!=0x0){
		borderColor->save("borderColor",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

SliderLAFType::SliderLAFType()
	:recColor(0x0),markColor(0x0),texColor(0x0),hoveredColor(0x0),defaultPercision(2){}

SliderLAFType::SliderLAFType(SliderLAFType&& other)
	:recColor(other.recColor),markColor(other.markColor),texColor(other.texColor),hoveredColor(other.hoveredColor),baseHH(other.baseHH),hh(other.hh),markLen(other.markLen),defaultPercision(other.defaultPercision)
{
	other.recColor=nullptr;
	other.markColor=nullptr;
	other.texColor=nullptr;
	other.hoveredColor=nullptr;
}
SliderLAFType::SliderLAFType(rapidxml::xml_node<>*  node)
	:recColor(0x0),markColor(0x0),texColor(0x0),hoveredColor(0x0),defaultPercision(2){
	setValue(&baseHH, node->first_attribute("baseHH"));
	setValue(&hh, node->first_attribute("hh"));
	setValue(&markLen, node->first_attribute("markLen"));
	setValue(&defaultPercision, node->first_attribute("defaultPercision"));
		create(&recColor, node->first_node("recColor"));
		create(&markColor, node->first_node("markColor"));
		create(&texColor, node->first_node("texColor"));
		create(&hoveredColor, node->first_node("hoveredColor"));
}

SliderLAFType::~SliderLAFType(){
	delete recColor;
	delete markColor;
	delete texColor;
	delete hoveredColor;
}

void SliderLAFType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" baseHH=\""<<baseHH<<"\"";
		out<<" hh=\""<<hh<<"\"";
		out<<" markLen=\""<<markLen<<"\"";
		out<<" defaultPercision=\""<<defaultPercision<<"\"";
		out<<">\n";
	}
	if(recColor!=0x0){
		recColor->save("recColor",out,nestingLevel+1);
	}
	if(markColor!=0x0){
		markColor->save("markColor",out,nestingLevel+1);
	}
	if(texColor!=0x0){
		texColor->save("texColor",out,nestingLevel+1);
	}
	if(hoveredColor!=0x0){
		hoveredColor->save("hoveredColor",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

