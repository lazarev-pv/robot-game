#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <cstring>
#include "util.h"
using namespace rapidxml;
using namespace std;
AllFilterType::AllFilterType()
	:maxSize(10){}

AllFilterType::AllFilterType(AllFilterType&& other)
	:maxSize(other.maxSize)
{
}
AllFilterType::AllFilterType(rapidxml::xml_node<>*  node)
	:maxSize(10){
	setValue(&maxSize, node->first_attribute("maxSize"));
}

AllFilterType::~AllFilterType(){
}

void AllFilterType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" maxSize=\""<<maxSize<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ButtonImageType::ButtonImageType()
	:normal(""),hovered(""),pushed(""){}

ButtonImageType::ButtonImageType(ButtonImageType&& other)
	:normal(other.normal),hovered(other.hovered),pushed(other.pushed)
{
}
ButtonImageType::ButtonImageType(rapidxml::xml_node<>*  node)
	:normal(""),hovered(""),pushed(""){
	setValue(&normal, node->first_attribute("normal"));
	setValue(&hovered, node->first_attribute("hovered"));
	setValue(&pushed, node->first_attribute("pushed"));
}

ButtonImageType::~ButtonImageType(){
}

void ButtonImageType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" normal=\""<<normal<<"\"";
		out<<" hovered=\""<<hovered<<"\"";
		out<<" pushed=\""<<pushed<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ButtonSlot::ButtonSlot()
	:image(0x0),text(""),script(""){}

ButtonSlot::ButtonSlot(ButtonSlot&& other)
	:image(other.image),text(other.text),script(other.script)
{
	other.image=nullptr;
}
ButtonSlot::ButtonSlot(rapidxml::xml_node<>*  node)
	:image(0x0),text(""),script(""){
	setValue(&text, node->first_attribute("text"));
	setValue(&script, node->first_attribute("script"));
		create(&image, node->first_node("image"));
}

ButtonSlot::~ButtonSlot(){
	delete image;
}

void ButtonSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" text=\""<<text<<"\"";
		out<<" script=\""<<script<<"\"";
		out<<">\n";
	}
	if(image!=0x0){
		image->save("image",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

CheckBoxSlot::CheckBoxSlot()
	:script(""){}

CheckBoxSlot::CheckBoxSlot(CheckBoxSlot&& other)
	:value(other.value),script(other.script)
{
}
CheckBoxSlot::CheckBoxSlot(rapidxml::xml_node<>*  node)
	:script(""){
	setValue(&value, node->first_attribute("value"));
	setValue(&script, node->first_attribute("script"));
}

CheckBoxSlot::~CheckBoxSlot(){
}

void CheckBoxSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" value=\""<<value<<"\"";
		out<<" script=\""<<script<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ContainerLineType::ContainerLineType()
	:slot(0x0),weight(""){}

ContainerLineType::ContainerLineType(ContainerLineType&& other)
	:slot(move(other.slot)),weight(other.weight)
{
}
ContainerLineType::ContainerLineType(rapidxml::xml_node<>*  node)
	:slot(0x0),weight(""){
	setValue(&weight, node->first_attribute("weight"));
	for(xml_node<>* currNode = node->first_node("slot");
			currNode!=0;
			currNode = currNode->next_sibling("slot")){
		slot.push_back(new SlotType(currNode));
	}
}

ContainerLineType::~ContainerLineType(){
	for(vector<SlotType* >::iterator it = slot.begin();
			it!=slot.end();++it){
		delete *it;
	}
;}

void ContainerLineType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" weight=\""<<weight<<"\"";
		out<<">\n";
	}
	for(auto iter:slot){
		iter->save("slot",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ContainerSelectorSlot::ContainerSelectorSlot()
	:slot(0x0),initial(""){}

ContainerSelectorSlot::ContainerSelectorSlot(ContainerSelectorSlot&& other)
	:slot(move(other.slot)),initial(other.initial)
{
}
ContainerSelectorSlot::ContainerSelectorSlot(rapidxml::xml_node<>*  node)
	:slot(0x0),initial(""){
	setValue(&initial, node->first_attribute("initial"));
	for(xml_node<>* currNode = node->first_node("slot");
			currNode!=0;
			currNode = currNode->next_sibling("slot")){
		slot.push_back(new SlotType(currNode));
	}
}

ContainerSelectorSlot::~ContainerSelectorSlot(){
	for(vector<SlotType* >::iterator it = slot.begin();
			it!=slot.end();++it){
		delete *it;
	}
;}

void ContainerSelectorSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" initial=\""<<initial<<"\"";
		out<<">\n";
	}
	for(auto iter:slot){
		iter->save("slot",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ContextMenuElemType::ContextMenuElemType()
	:elem(0x0),name(""){}

ContextMenuElemType::ContextMenuElemType(ContextMenuElemType&& other)
	:elem(move(other.elem)),name(other.name)
{
}
ContextMenuElemType::ContextMenuElemType(rapidxml::xml_node<>*  node)
	:elem(0x0),name(""){
	setValue(&name, node->first_attribute("name"));
	for(xml_node<>* currNode = node->first_node("elem");
			currNode!=0;
			currNode = currNode->next_sibling("elem")){
		elem.push_back(new ContextMenuElemType(currNode));
	}
}

ContextMenuElemType::~ContextMenuElemType(){
	for(vector<ContextMenuElemType* >::iterator it = elem.begin();
			it!=elem.end();++it){
		delete *it;
	}
;}

void ContextMenuElemType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<">\n";
	}
	for(auto iter:elem){
		iter->save("elem",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ContextMenuSlot::ContextMenuSlot()
	:elem(0x0){}

ContextMenuSlot::ContextMenuSlot(ContextMenuSlot&& other)
	:elem(move(other.elem))
{
}
ContextMenuSlot::ContextMenuSlot(rapidxml::xml_node<>*  node)
	:elem(0x0){
	for(xml_node<>* currNode = node->first_node("elem");
			currNode!=0;
			currNode = currNode->next_sibling("elem")){
		elem.push_back(new ContextMenuElemType(currNode));
	}
}

ContextMenuSlot::~ContextMenuSlot(){
	for(vector<ContextMenuElemType* >::iterator it = elem.begin();
			it!=elem.end();++it){
		delete *it;
	}
;}

void ContextMenuSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:elem){
		iter->save("elem",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

DDLBElemType::DDLBElemType()
	:name(""){}

DDLBElemType::DDLBElemType(DDLBElemType&& other)
	:name(other.name)
{
}
DDLBElemType::DDLBElemType(rapidxml::xml_node<>*  node)
	:name(""){
	setValue(&name, node->first_attribute("name"));
}

DDLBElemType::~DDLBElemType(){
}

void DDLBElemType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<"/>";
	}
	out<<endl;
}

DDLBSlot::DDLBSlot()
	:elem(0x0),text(""){}

DDLBSlot::DDLBSlot(DDLBSlot&& other)
	:elem(move(other.elem)),text(other.text)
{
}
DDLBSlot::DDLBSlot(rapidxml::xml_node<>*  node)
	:elem(0x0),text(""){
	setValue(&text, node->first_attribute("text"));
	for(xml_node<>* currNode = node->first_node("elem");
			currNode!=0;
			currNode = currNode->next_sibling("elem")){
		elem.push_back(new DDLBElemType(currNode));
	}
}

DDLBSlot::~DDLBSlot(){
	for(vector<DDLBElemType* >::iterator it = elem.begin();
			it!=elem.end();++it){
		delete *it;
	}
;}

void DDLBSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" text=\""<<text<<"\"";
		out<<">\n";
	}
	for(auto iter:elem){
		iter->save("elem",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

DialogSlot::DialogSlot()
	:slot(0x0),capture(""){}

DialogSlot::DialogSlot(DialogSlot&& other)
	:slot(move(other.slot)),capture(other.capture)
{
}
DialogSlot::DialogSlot(rapidxml::xml_node<>*  node)
	:slot(0x0),capture(""){
	setValue(&capture, node->first_attribute("capture"));
	for(xml_node<>* currNode = node->first_node("slot");
			currNode!=0;
			currNode = currNode->next_sibling("slot")){
		slot.push_back(new SlotType(currNode));
	}
}

DialogSlot::~DialogSlot(){
	for(vector<SlotType* >::iterator it = slot.begin();
			it!=slot.end();++it){
		delete *it;
	}
;}

void DialogSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" capture=\""<<capture<<"\"";
		out<<">\n";
	}
	for(auto iter:slot){
		iter->save("slot",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

HealthbarSlot::HealthbarSlot()
	:color(0x0),percision(-1){}

HealthbarSlot::HealthbarSlot(HealthbarSlot&& other)
	:color(other.color),min(other.min),max(other.max),value(other.value),percision(other.percision)
{
	other.color=nullptr;
}
HealthbarSlot::HealthbarSlot(rapidxml::xml_node<>*  node)
	:color(0x0),percision(-1){
	setValue(&min, node->first_attribute("min"));
	setValue(&max, node->first_attribute("max"));
	setValue(&value, node->first_attribute("value"));
	setValue(&percision, node->first_attribute("percision"));
		create(&color, node->first_node("color"));
}

HealthbarSlot::~HealthbarSlot(){
	delete color;
}

void HealthbarSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" min=\""<<min<<"\"";
		out<<" max=\""<<max<<"\"";
		out<<" value=\""<<value<<"\"";
		out<<" percision=\""<<percision<<"\"";
		out<<">\n";
	}
	if(color!=0x0){
		color->save("color",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ImagePickerSlot::ImagePickerSlot(){}

ImagePickerSlot::ImagePickerSlot(ImagePickerSlot&& other)
{
}
ImagePickerSlot::ImagePickerSlot(rapidxml::xml_node<>*  ){
}

ImagePickerSlot::~ImagePickerSlot(){
}

void ImagePickerSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<"/>";
	}
	out<<endl;
}

ImageSlot::ImageSlot()
	:presentation(""){}

ImageSlot::ImageSlot(ImageSlot&& other)
	:presentation(other.presentation)
{
}
ImageSlot::ImageSlot(rapidxml::xml_node<>*  node)
	:presentation(""){
	setValue(&presentation, node->first_attribute("presentation"));
}

ImageSlot::~ImageSlot(){
}

void ImageSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" presentation=\""<<presentation<<"\"";
		out<<"/>";
	}
	out<<endl;
}

InputSlot::InputSlot()
	:value(""){}

InputSlotUnion::InputSlotUnion()
	:onlyDigits(0x0),all(0x0){}

InputSlot::InputSlot(InputSlot&& other)
	:choiceValue(move(other.choiceValue)),value(other.value){
}
InputSlotUnion::InputSlotUnion(InputSlotUnion&& other)
	:onlyDigits(other.onlyDigits),all(other.all)
{
	other.onlyDigits=nullptr;
	other.all=nullptr;
}
InputSlot::InputSlot(rapidxml::xml_node<>*  node)
	:value(""){
	setValue(&value, node->first_attribute("value"));
	xml_node<>* choiceNode = node->first_node();
	if(choiceNode!=0){
		if(strcmp(choiceNode->name(),"onlyDigits")==0){
			create(&choiceValue.onlyDigits, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"all")==0){
			create(&choiceValue.all, choiceNode);
		}
	}
}
InputSlot::~InputSlot(){
	delete choiceValue.onlyDigits;
	delete choiceValue.all;
}

void InputSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" value=\""<<value<<"\"";
		out<<">\n";
	}
	if(choiceValue.onlyDigits!=0x0){
		choiceValue.onlyDigits->save("onlyDigits",out,nestingLevel+1);
	}
	if(choiceValue.all!=0x0){
		choiceValue.all->save("all",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

LabelsListSlot::LabelsListSlot()
	:elem(0x0){}

LabelsListSlot::LabelsListSlot(LabelsListSlot&& other)
	:elem(move(other.elem))
{
}
LabelsListSlot::LabelsListSlot(rapidxml::xml_node<>*  node)
	:elem(0x0){
	for(xml_node<>* currNode = node->first_node("elem");
			currNode!=0;
			currNode = currNode->next_sibling("elem")){
		elem.push_back(new TextSlot(currNode));
	}
}

LabelsListSlot::~LabelsListSlot(){
	for(vector<TextSlot* >::iterator it = elem.begin();
			it!=elem.end();++it){
		delete *it;
	}
;}

void LabelsListSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:elem){
		iter->save("elem",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

LineContainerSlot::LineContainerSlot()
	:backColor(0x0),line(0x0),caption(""){}

LineContainerSlot::LineContainerSlot(LineContainerSlot&& other)
	:backColor(other.backColor),line(move(other.line)),yIntent(other.yIntent),xIntent(other.xIntent),caption(other.caption)
{
	other.backColor=nullptr;
}
LineContainerSlot::LineContainerSlot(rapidxml::xml_node<>*  node)
	:backColor(0x0),line(0x0),caption(""){
	setValue(&yIntent, node->first_attribute("yIntent"));
	setValue(&xIntent, node->first_attribute("xIntent"));
	setValue(&caption, node->first_attribute("caption"));
		create(&backColor, node->first_node("backColor"));
	for(xml_node<>* currNode = node->first_node("line");
			currNode!=0;
			currNode = currNode->next_sibling("line")){
		line.push_back(new ContainerLineType(currNode));
	}
}

LineContainerSlot::~LineContainerSlot(){
	delete backColor;
	for(vector<ContainerLineType* >::iterator it = line.begin();
			it!=line.end();++it){
		delete *it;
	}
;}

void LineContainerSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" yIntent=\""<<yIntent<<"\"";
		out<<" xIntent=\""<<xIntent<<"\"";
		out<<" caption=\""<<caption<<"\"";
		out<<">\n";
	}
	if(backColor!=0x0){
		backColor->save("backColor",out,nestingLevel+1);
	}
	for(auto iter:line){
		iter->save("line",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

MenuType::MenuType()
	:elem(0x0),name(""){}

MenuType::MenuType(MenuType&& other)
	:elem(move(other.elem)),name(other.name)
{
}
MenuType::MenuType(rapidxml::xml_node<>*  node)
	:elem(0x0),name(""){
	setValue(&name, node->first_attribute("name"));
	for(xml_node<>* currNode = node->first_node("elem");
			currNode!=0;
			currNode = currNode->next_sibling("elem")){
		elem.push_back(new ContextMenuElemType(currNode));
	}
}

MenuType::~MenuType(){
	for(vector<ContextMenuElemType* >::iterator it = elem.begin();
			it!=elem.end();++it){
		delete *it;
	}
;}

void MenuType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<">\n";
	}
	for(auto iter:elem){
		iter->save("elem",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

OnlyDigitsFilterType::OnlyDigitsFilterType(){}

OnlyDigitsFilterType::OnlyDigitsFilterType(OnlyDigitsFilterType&& other)
	:maxSize(other.maxSize)
{
}
OnlyDigitsFilterType::OnlyDigitsFilterType(rapidxml::xml_node<>*  node){
	setValue(&maxSize, node->first_attribute("maxSize"));
}

OnlyDigitsFilterType::~OnlyDigitsFilterType(){
}

void OnlyDigitsFilterType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" maxSize=\""<<maxSize<<"\"";
		out<<"/>";
	}
	out<<endl;
}

RGBSlot::RGBSlot()
	:color(0x0){}

RGBSlot::RGBSlot(RGBSlot&& other)
	:color(other.color)
{
	other.color=nullptr;
}
RGBSlot::RGBSlot(rapidxml::xml_node<>*  node)
	:color(0x0){
		create(&color, node->first_node("color"));
}

RGBSlot::~RGBSlot(){
	delete color;
}

void RGBSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(color!=0x0){
		color->save("color",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

RadioSlot::RadioSlot()
	:group(""){}

RadioSlot::RadioSlot(RadioSlot&& other)
	:value(other.value),group(other.group)
{
}
RadioSlot::RadioSlot(rapidxml::xml_node<>*  node)
	:group(""){
	setValue(&value, node->first_attribute("value"));
	setValue(&group, node->first_attribute("group"));
}

RadioSlot::~RadioSlot(){
}

void RadioSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" value=\""<<value<<"\"";
		out<<" group=\""<<group<<"\"";
		out<<"/>";
	}
	out<<endl;
}

SliderSlot::SliderSlot()
	:percision(-1){}

SliderSlot::SliderSlot(SliderSlot&& other)
	:min(other.min),max(other.max),value(other.value),percision(other.percision)
{
}
SliderSlot::SliderSlot(rapidxml::xml_node<>*  node)
	:percision(-1){
	setValue(&min, node->first_attribute("min"));
	setValue(&max, node->first_attribute("max"));
	setValue(&value, node->first_attribute("value"));
	setValue(&percision, node->first_attribute("percision"));
}

SliderSlot::~SliderSlot(){
}

void SliderSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" min=\""<<min<<"\"";
		out<<" max=\""<<max<<"\"";
		out<<" value=\""<<value<<"\"";
		out<<" percision=\""<<percision<<"\"";
		out<<"/>";
	}
	out<<endl;
}

SlotType::SlotType()
	:x(0),y(0),z(0),hw(-1),hh(-1),name(""),visible(true),enabled(true){}

SlotTypeUnion::SlotTypeUnion()
	:button(0x0),text(0x0),image(0x0),ddlb(0x0),input(0x0),context(0x0),dialog(0x0),imagePicker(0x0),checkbox(0x0),slider(0x0),rgb(0x0),radio(0x0),labelsList(0x0),tableContainer(0x0),lineContainer(0x0),containerSelector(0x0),healthbar(0x0){}

SlotType::SlotType(SlotType&& other)
	:choiceValue(move(other.choiceValue)),x(other.x),y(other.y),z(other.z),hw(other.hw),hh(other.hh),name(other.name),visible(other.visible),enabled(other.enabled){
}
SlotTypeUnion::SlotTypeUnion(SlotTypeUnion&& other)
	:button(other.button),text(other.text),image(other.image),ddlb(other.ddlb),input(other.input),context(other.context),dialog(other.dialog),imagePicker(other.imagePicker),checkbox(other.checkbox),slider(other.slider),rgb(other.rgb),radio(other.radio),labelsList(other.labelsList),tableContainer(other.tableContainer),lineContainer(other.lineContainer),containerSelector(other.containerSelector),healthbar(other.healthbar)
{
	other.button=nullptr;
	other.text=nullptr;
	other.image=nullptr;
	other.ddlb=nullptr;
	other.input=nullptr;
	other.context=nullptr;
	other.dialog=nullptr;
	other.imagePicker=nullptr;
	other.checkbox=nullptr;
	other.slider=nullptr;
	other.rgb=nullptr;
	other.radio=nullptr;
	other.labelsList=nullptr;
	other.tableContainer=nullptr;
	other.lineContainer=nullptr;
	other.containerSelector=nullptr;
	other.healthbar=nullptr;
}
SlotType::SlotType(rapidxml::xml_node<>*  node)
	:x(0),y(0),z(0),hw(-1),hh(-1),name(""),visible(true),enabled(true){
	setValue(&x, node->first_attribute("x"));
	setValue(&y, node->first_attribute("y"));
	setValue(&z, node->first_attribute("z"));
	setValue(&hw, node->first_attribute("hw"));
	setValue(&hh, node->first_attribute("hh"));
	setValue(&name, node->first_attribute("name"));
	setValue(&visible, node->first_attribute("visible"));
	setValue(&enabled, node->first_attribute("enabled"));
	xml_node<>* choiceNode = node->first_node();
	if(choiceNode!=0){
		if(strcmp(choiceNode->name(),"button")==0){
			create(&choiceValue.button, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"text")==0){
			create(&choiceValue.text, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"image")==0){
			create(&choiceValue.image, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"ddlb")==0){
			create(&choiceValue.ddlb, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"input")==0){
			create(&choiceValue.input, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"context")==0){
			create(&choiceValue.context, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"dialog")==0){
			create(&choiceValue.dialog, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"imagePicker")==0){
			create(&choiceValue.imagePicker, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"checkbox")==0){
			create(&choiceValue.checkbox, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"slider")==0){
			create(&choiceValue.slider, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"rgb")==0){
			create(&choiceValue.rgb, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"radio")==0){
			create(&choiceValue.radio, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"labelsList")==0){
			create(&choiceValue.labelsList, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"tableContainer")==0){
			create(&choiceValue.tableContainer, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"lineContainer")==0){
			create(&choiceValue.lineContainer, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"containerSelector")==0){
			create(&choiceValue.containerSelector, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"healthbar")==0){
			create(&choiceValue.healthbar, choiceNode);
		}
	}
}
SlotType::~SlotType(){
	delete choiceValue.button;
	delete choiceValue.text;
	delete choiceValue.image;
	delete choiceValue.ddlb;
	delete choiceValue.input;
	delete choiceValue.context;
	delete choiceValue.dialog;
	delete choiceValue.imagePicker;
	delete choiceValue.checkbox;
	delete choiceValue.slider;
	delete choiceValue.rgb;
	delete choiceValue.radio;
	delete choiceValue.labelsList;
	delete choiceValue.tableContainer;
	delete choiceValue.lineContainer;
	delete choiceValue.containerSelector;
	delete choiceValue.healthbar;
}

void SlotType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" x=\""<<x<<"\"";
		out<<" y=\""<<y<<"\"";
		out<<" z=\""<<z<<"\"";
		out<<" hw=\""<<hw<<"\"";
		out<<" hh=\""<<hh<<"\"";
		out<<" name=\""<<name<<"\"";
		out<<" visible=\""<<visible<<"\"";
		out<<" enabled=\""<<enabled<<"\"";
		out<<">\n";
	}
	if(choiceValue.button!=0x0){
		choiceValue.button->save("button",out,nestingLevel+1);
	}
	if(choiceValue.text!=0x0){
		choiceValue.text->save("text",out,nestingLevel+1);
	}
	if(choiceValue.image!=0x0){
		choiceValue.image->save("image",out,nestingLevel+1);
	}
	if(choiceValue.ddlb!=0x0){
		choiceValue.ddlb->save("ddlb",out,nestingLevel+1);
	}
	if(choiceValue.input!=0x0){
		choiceValue.input->save("input",out,nestingLevel+1);
	}
	if(choiceValue.context!=0x0){
		choiceValue.context->save("context",out,nestingLevel+1);
	}
	if(choiceValue.dialog!=0x0){
		choiceValue.dialog->save("dialog",out,nestingLevel+1);
	}
	if(choiceValue.imagePicker!=0x0){
		choiceValue.imagePicker->save("imagePicker",out,nestingLevel+1);
	}
	if(choiceValue.checkbox!=0x0){
		choiceValue.checkbox->save("checkbox",out,nestingLevel+1);
	}
	if(choiceValue.slider!=0x0){
		choiceValue.slider->save("slider",out,nestingLevel+1);
	}
	if(choiceValue.rgb!=0x0){
		choiceValue.rgb->save("rgb",out,nestingLevel+1);
	}
	if(choiceValue.radio!=0x0){
		choiceValue.radio->save("radio",out,nestingLevel+1);
	}
	if(choiceValue.labelsList!=0x0){
		choiceValue.labelsList->save("labelsList",out,nestingLevel+1);
	}
	if(choiceValue.tableContainer!=0x0){
		choiceValue.tableContainer->save("tableContainer",out,nestingLevel+1);
	}
	if(choiceValue.lineContainer!=0x0){
		choiceValue.lineContainer->save("lineContainer",out,nestingLevel+1);
	}
	if(choiceValue.containerSelector!=0x0){
		choiceValue.containerSelector->save("containerSelector",out,nestingLevel+1);
	}
	if(choiceValue.healthbar!=0x0){
		choiceValue.healthbar->save("healthbar",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

TableContainerSlot::TableContainerSlot()
	:backColor(0x0),slot(0x0),drawLines(false),align(""),caption(""){}

TableContainerSlot::TableContainerSlot(TableContainerSlot&& other)
	:backColor(other.backColor),weight(move(other.weight)),slot(move(other.slot)),intent(other.intent),drawLines(other.drawLines),align(other.align),caption(other.caption)
{
	other.backColor=nullptr;
}
TableContainerSlot::TableContainerSlot(rapidxml::xml_node<>*  node)
	:backColor(0x0),slot(0x0),drawLines(false),align(""),caption(""){
	setValue(&intent, node->first_attribute("intent"));
	setValue(&drawLines, node->first_attribute("drawLines"));
	setValue(&align, node->first_attribute("align"));
	setValue(&caption, node->first_attribute("caption"));
		create(&backColor, node->first_node("backColor"));
	for(xml_node<>* currNode = node->first_node("weight");
			currNode!=0;
			currNode = currNode->next_sibling("weight")){
		float tmp;
		setValue(&tmp, currNode);
		weight.push_back(tmp);
	}
	for(xml_node<>* currNode = node->first_node("slot");
			currNode!=0;
			currNode = currNode->next_sibling("slot")){
		slot.push_back(new SlotType(currNode));
	}
}

TableContainerSlot::~TableContainerSlot(){
	delete backColor;
	for(vector<SlotType* >::iterator it = slot.begin();
			it!=slot.end();++it){
		delete *it;
	}
;}

void TableContainerSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" intent=\""<<intent<<"\"";
		out<<" drawLines=\""<<drawLines<<"\"";
		out<<" align=\""<<align<<"\"";
		out<<" caption=\""<<caption<<"\"";
		out<<">\n";
	}
	if(backColor!=0x0){
		backColor->save("backColor",out,nestingLevel+1);
	}
	for(auto iter:weight){
		out<<nestPrefix;
		createTag(out,"weight",iter);
	}
	for(auto iter:slot){
		iter->save("slot",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

TextSlot::TextSlot()
	:text(""){}

TextSlot::TextSlot(TextSlot&& other)
	:text(other.text)
{
}
TextSlot::TextSlot(rapidxml::xml_node<>*  node)
	:text(""){
	setValue(&text, node->first_attribute("text"));
}

TextSlot::~TextSlot(){
}

void TextSlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" text=\""<<text<<"\"";
		out<<"/>";
	}
	out<<endl;
}

UserInterfaceParserRoot::UserInterfaceParserRoot()
	:userInterface(0x0){}

UserInterfaceParserRoot::UserInterfaceParserRoot(UserInterfaceParserRoot&& other)
	:userInterface(other.userInterface)
{
	other.userInterface=nullptr;
}
UserInterfaceParserRoot::UserInterfaceParserRoot(rapidxml::xml_node<>*  node)
	:userInterface(0x0){
		create(&userInterface, node->first_node("userInterface"));
}

UserInterfaceParserRoot::~UserInterfaceParserRoot(){
	delete userInterface;
}

void UserInterfaceParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(userInterface!=0x0){
		userInterface->save("userInterface",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

UserInterfaceType::UserInterfaceType()
	:menu(0x0),slot(0x0),scriptsFile(""),lookAndFeel(""){}

UserInterfaceType::UserInterfaceType(UserInterfaceType&& other)
	:menu(other.menu),slot(move(other.slot)),scriptsFile(other.scriptsFile),lookAndFeel(other.lookAndFeel)
{
	other.menu=nullptr;
}
UserInterfaceType::UserInterfaceType(rapidxml::xml_node<>*  node)
	:menu(0x0),slot(0x0),scriptsFile(""),lookAndFeel(""){
	setValue(&scriptsFile, node->first_attribute("scriptsFile"));
	setValue(&lookAndFeel, node->first_attribute("lookAndFeel"));
		create(&menu, node->first_node("menu"));
	for(xml_node<>* currNode = node->first_node("slot");
			currNode!=0;
			currNode = currNode->next_sibling("slot")){
		slot.push_back(new SlotType(currNode));
	}
}

UserInterfaceType::~UserInterfaceType(){
	delete menu;
	for(vector<SlotType* >::iterator it = slot.begin();
			it!=slot.end();++it){
		delete *it;
	}
;}

void UserInterfaceType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" scriptsFile=\""<<scriptsFile<<"\"";
		out<<" lookAndFeel=\""<<lookAndFeel<<"\"";
		out<<">\n";
	}
	if(menu!=0x0){
		menu->save("menu",out,nestingLevel+1);
	}
	for(auto iter:slot){
		iter->save("slot",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

