#include <cstring>
#include "SaveParser.h"
#include "util.h"
using namespace rapidxml;
using namespace std;
ChedSavedData::ChedSavedData()
	:timeLine(0x0),properties(0x0),draw(0x0),camera(0x0),windows(0x0){}

ChedSavedData::ChedSavedData(ChedSavedData&& other)
	:timeLine(other.timeLine),properties(other.properties),draw(other.draw),camera(other.camera),windows(move(other.windows))
{
	other.timeLine=nullptr;
	other.properties=nullptr;
	other.draw=nullptr;
	other.camera=nullptr;
}
ChedSavedData::ChedSavedData(rapidxml::xml_node<>*  node)
	:timeLine(0x0),properties(0x0),draw(0x0),camera(0x0),windows(0x0){
		create(&timeLine, node->first_node("timeLine"));
		create(&properties, node->first_node("properties"));
		create(&draw, node->first_node("draw"));
		create(&camera, node->first_node("camera"));
	for(xml_node<>* currNode = node->first_node("windows");
			currNode!=0;
			currNode = currNode->next_sibling("windows")){
		windows.push_back(new WindowPropsType(currNode));
	}
}

ChedSavedData::~ChedSavedData(){
	delete timeLine;
	delete properties;
	delete draw;
	delete camera;
	for(vector<WindowPropsType* >::iterator it = windows.begin();
			it!=windows.end();++it){
		delete *it;
	}
;}

void ChedSavedData::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(timeLine!=0x0){
		timeLine->save("timeLine",out,nestingLevel+1);
	}
	if(properties!=0x0){
		properties->save("properties",out,nestingLevel+1);
	}
	if(draw!=0x0){
		draw->save("draw",out,nestingLevel+1);
	}
	if(camera!=0x0){
		camera->save("camera",out,nestingLevel+1);
	}
	for(auto iter:windows){
		iter->save("windows",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

PropEntityStateType::PropEntityStateType(){}

PropEntityStateType::PropEntityStateType(PropEntityStateType&& other)
	:drawable(other.drawable),selectable(other.selectable)
{
}
PropEntityStateType::PropEntityStateType(rapidxml::xml_node<>*  node){
	setValue(&drawable, node->first_attribute("drawable"));
	setValue(&selectable, node->first_attribute("selectable"));
}

PropEntityStateType::~PropEntityStateType(){
}

void PropEntityStateType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" drawable=\""<<drawable<<"\"";
		out<<" selectable=\""<<selectable<<"\"";
		out<<"/>";
	}
	out<<endl;
}

SaveParserRoot::SaveParserRoot()
	:savedData(0x0){}

SaveParserRoot::SaveParserRoot(SaveParserRoot&& other)
	:savedData(other.savedData)
{
	other.savedData=nullptr;
}
SaveParserRoot::SaveParserRoot(rapidxml::xml_node<>*  node)
	:savedData(0x0){
		create(&savedData, node->first_node("savedData"));
}

SaveParserRoot::~SaveParserRoot(){
	delete savedData;
}

void SaveParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(savedData!=0x0){
		savedData->save("savedData",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

SavedDrawType::SavedDrawType()
	:bonesState(0x0),picsState(0x0),physState(0x0){}

SavedDrawType::SavedDrawType(SavedDrawType&& other)
	:bonesState(other.bonesState),picsState(other.picsState),physState(other.physState)
{
	other.bonesState=nullptr;
	other.picsState=nullptr;
	other.physState=nullptr;
}
SavedDrawType::SavedDrawType(rapidxml::xml_node<>*  node)
	:bonesState(0x0),picsState(0x0),physState(0x0){
		create(&bonesState, node->first_node("bonesState"));
		create(&picsState, node->first_node("picsState"));
		create(&physState, node->first_node("physState"));
}

SavedDrawType::~SavedDrawType(){
	delete bonesState;
	delete picsState;
	delete physState;
}

void SavedDrawType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(bonesState!=0x0){
		bonesState->save("bonesState",out,nestingLevel+1);
	}
	if(picsState!=0x0){
		picsState->save("picsState",out,nestingLevel+1);
	}
	if(physState!=0x0){
		physState->save("physState",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

SavedPropertiesType::SavedPropertiesType()
	:animation(""),workTarget(""){}

SavedPropertiesType::SavedPropertiesType(SavedPropertiesType&& other)
	:animation(other.animation),workTarget(other.workTarget)
{
}
SavedPropertiesType::SavedPropertiesType(rapidxml::xml_node<>*  node)
	:animation(""),workTarget(""){
	setValue(&animation, node->first_attribute("animation"));
	setValue(&workTarget, node->first_attribute("workTarget"));
}

SavedPropertiesType::~SavedPropertiesType(){
}

void SavedPropertiesType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" animation=\""<<animation<<"\"";
		out<<" workTarget=\""<<workTarget<<"\"";
		out<<"/>";
	}
	out<<endl;
}

TimeLineType::TimeLineType(){}

TimeLineType::TimeLineType(TimeLineType&& other)
	:startX(other.startX),endX(other.endX),divStep(other.divStep),markFreq(other.markFreq)
{
}
TimeLineType::TimeLineType(rapidxml::xml_node<>*  node){
		setValue(&startX, node->first_node("startX"));
		setValue(&endX, node->first_node("endX"));
		setValue(&divStep, node->first_node("divStep"));
		setValue(&markFreq, node->first_node("markFreq"));
}

TimeLineType::~TimeLineType(){
}

void TimeLineType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
		out<<nestPrefix;
		createTag(out,"startX",startX);
		out<<nestPrefix;
		createTag(out,"endX",endX);
		out<<nestPrefix;
		createTag(out,"divStep",divStep);
		out<<nestPrefix;
		createTag(out,"markFreq",markFreq);
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

WindowPropsType::WindowPropsType()
	:pos(0x0),name(""),minimalized(false),visible(true){}

WindowPropsType::WindowPropsType(WindowPropsType&& other)
	:pos(other.pos),name(other.name),minimalized(other.minimalized),visible(other.visible)
{
	other.pos=nullptr;
}
WindowPropsType::WindowPropsType(rapidxml::xml_node<>*  node)
	:pos(0x0),name(""),minimalized(false),visible(true){
	setValue(&name, node->first_attribute("name"));
	setValue(&minimalized, node->first_attribute("minimalized"));
	setValue(&visible, node->first_attribute("visible"));
		create(&pos, node->first_node("pos"));
}

WindowPropsType::~WindowPropsType(){
	delete pos;
}

void WindowPropsType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" minimalized=\""<<minimalized<<"\"";
		out<<" visible=\""<<visible<<"\"";
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

