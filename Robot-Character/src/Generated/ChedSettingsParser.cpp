#include <cstring>
#include "ChedSettingsParser.h"
#include "util.h"
using namespace rapidxml;
using namespace std;
ChedSettingsParserRoot::ChedSettingsParserRoot()
	:chedSettings(0x0){}

ChedSettingsParserRoot::ChedSettingsParserRoot(ChedSettingsParserRoot&& other)
	:chedSettings(other.chedSettings)
{
	other.chedSettings=nullptr;
}
ChedSettingsParserRoot::ChedSettingsParserRoot(rapidxml::xml_node<>*  node)
	:chedSettings(0x0){
		create(&chedSettings, node->first_node("chedSettings"));
}

ChedSettingsParserRoot::~ChedSettingsParserRoot(){
	delete chedSettings;
}

void ChedSettingsParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(chedSettings!=0x0){
		chedSettings->save("chedSettings",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ChedSettingsType::ChedSettingsType()
	:timeline(0x0){}

ChedSettingsType::ChedSettingsType(ChedSettingsType&& other)
	:timeline(other.timeline)
{
	other.timeline=nullptr;
}
ChedSettingsType::ChedSettingsType(rapidxml::xml_node<>*  node)
	:timeline(0x0){
		create(&timeline, node->first_node("timeline"));
}

ChedSettingsType::~ChedSettingsType(){
	delete timeline;
}

void ChedSettingsType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(timeline!=0x0){
		timeline->save("timeline",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ChedTimeLine::ChedTimeLine()
	:pos(0x0),background(""),linesHexColor(""),labelHexColor(""),frameHexColor(""),animEndHexColor(""),currentTimeHexColor(""){}

ChedTimeLine::ChedTimeLine(ChedTimeLine&& other)
	:pos(other.pos),hw(other.hw),hh(other.hh),xPadding(other.xPadding),yPadding(other.yPadding),elementsCount(other.elementsCount),numPerElement(other.numPerElement),labelSize(other.labelSize),labelYOffset(other.labelYOffset),background(other.background),linesHexColor(other.linesHexColor),labelHexColor(other.labelHexColor),frameHexColor(other.frameHexColor),animEndHexColor(other.animEndHexColor),frameHW(other.frameHW),currentTimeHexColor(other.currentTimeHexColor),currentHW(other.currentHW),lineHeightPercent(other.lineHeightPercent),midLineHeightPercent(other.midLineHeightPercent)
{
	other.pos=nullptr;
}
ChedTimeLine::ChedTimeLine(rapidxml::xml_node<>*  node)
	:pos(0x0),background(""),linesHexColor(""),labelHexColor(""),frameHexColor(""),animEndHexColor(""),currentTimeHexColor(""){
	setValue(&hw, node->first_attribute("hw"));
	setValue(&hh, node->first_attribute("hh"));
	setValue(&xPadding, node->first_attribute("xPadding"));
	setValue(&yPadding, node->first_attribute("yPadding"));
	setValue(&elementsCount, node->first_attribute("elementsCount"));
	setValue(&numPerElement, node->first_attribute("numPerElement"));
	setValue(&labelSize, node->first_attribute("labelSize"));
	setValue(&labelYOffset, node->first_attribute("labelYOffset"));
	setValue(&background, node->first_attribute("background"));
	setValue(&linesHexColor, node->first_attribute("linesHexColor"));
	setValue(&labelHexColor, node->first_attribute("labelHexColor"));
	setValue(&frameHexColor, node->first_attribute("frameHexColor"));
	setValue(&animEndHexColor, node->first_attribute("animEndHexColor"));
	setValue(&frameHW, node->first_attribute("frameHW"));
	setValue(&currentTimeHexColor, node->first_attribute("currentTimeHexColor"));
	setValue(&currentHW, node->first_attribute("currentHW"));
	setValue(&lineHeightPercent, node->first_attribute("lineHeightPercent"));
	setValue(&midLineHeightPercent, node->first_attribute("midLineHeightPercent"));
		create(&pos, node->first_node("pos"));
}

ChedTimeLine::~ChedTimeLine(){
	delete pos;
}

void ChedTimeLine::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" hw=\""<<hw<<"\"";
		out<<" hh=\""<<hh<<"\"";
		out<<" xPadding=\""<<xPadding<<"\"";
		out<<" yPadding=\""<<yPadding<<"\"";
		out<<" elementsCount=\""<<elementsCount<<"\"";
		out<<" numPerElement=\""<<numPerElement<<"\"";
		out<<" labelSize=\""<<labelSize<<"\"";
		out<<" labelYOffset=\""<<labelYOffset<<"\"";
		out<<" background=\""<<background<<"\"";
		out<<" linesHexColor=\""<<linesHexColor<<"\"";
		out<<" labelHexColor=\""<<labelHexColor<<"\"";
		out<<" frameHexColor=\""<<frameHexColor<<"\"";
		out<<" animEndHexColor=\""<<animEndHexColor<<"\"";
		out<<" frameHW=\""<<frameHW<<"\"";
		out<<" currentTimeHexColor=\""<<currentTimeHexColor<<"\"";
		out<<" currentHW=\""<<currentHW<<"\"";
		out<<" lineHeightPercent=\""<<lineHeightPercent<<"\"";
		out<<" midLineHeightPercent=\""<<midLineHeightPercent<<"\"";
		out<<">\n";
	}
	if(pos!=0x0){
		pos->save("pos",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

