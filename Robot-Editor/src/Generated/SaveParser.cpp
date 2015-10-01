#include <cstring>
#include "SaveParser.h"
#include "util.h"
using namespace rapidxml;
using namespace std;
SaveDataType::SaveDataType()
	:selectFilter(0x0),cameraPos(0x0){}

SaveDataType::SaveDataType(SaveDataType&& other)
	:selectFilter(other.selectFilter),cameraPos(other.cameraPos)
{
	other.selectFilter=nullptr;
	other.cameraPos=nullptr;
}
SaveDataType::SaveDataType(rapidxml::xml_node<>*  node)
	:selectFilter(0x0),cameraPos(0x0){
		create(&selectFilter, node->first_node("selectFilter"));
		create(&cameraPos, node->first_node("cameraPos"));
}

SaveDataType::~SaveDataType(){
	delete selectFilter;
	delete cameraPos;
}

void SaveDataType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(selectFilter!=0x0){
		selectFilter->save("selectFilter",out,nestingLevel+1);
	}
	if(cameraPos!=0x0){
		cameraPos->save("cameraPos",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

SaveParserRoot::SaveParserRoot()
	:saveData(0x0){}

SaveParserRoot::SaveParserRoot(SaveParserRoot&& other)
	:saveData(other.saveData)
{
	other.saveData=nullptr;
}
SaveParserRoot::SaveParserRoot(rapidxml::xml_node<>*  node)
	:saveData(0x0){
		create(&saveData, node->first_node("saveData"));
}

SaveParserRoot::~SaveParserRoot(){
	delete saveData;
}

void SaveParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(saveData!=0x0){
		saveData->save("saveData",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

SelectFilterType::SelectFilterType(){}

SelectFilterType::SelectFilterType(SelectFilterType&& other)
	:entityName(move(other.entityName))
{
}
SelectFilterType::SelectFilterType(rapidxml::xml_node<>*  node){
	for(xml_node<>* currNode = node->first_node("entityName");
			currNode!=0;
			currNode = currNode->next_sibling("entityName")){
		std::string tmp;
		setValue(&tmp, currNode);
		entityName.push_back(tmp);
	}
}

SelectFilterType::~SelectFilterType(){
}

void SelectFilterType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:entityName){
		out<<nestPrefix;
		createTag(out,"entityName",iter);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

