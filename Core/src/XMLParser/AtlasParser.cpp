#include <ChibiEngine/XMLParser/AtlasParser.h>
#include <cstring>
#include "util.h"
using namespace rapidxml;
using namespace std;
AnimationType::AnimationType()
	:frame(0x0),name(""),cycled(false),frameTime(1){}

AnimationType::AnimationType(AnimationType&& other)
	:frame(move(other.frame)),name(other.name),cycled(other.cycled),frameTime(other.frameTime)
{
}
AnimationType::AnimationType(rapidxml::xml_node<>*  node)
	:frame(0x0),name(""),cycled(false),frameTime(1){
	setValue(&name, node->first_attribute("name"));
	setValue(&cycled, node->first_attribute("cycled"));
	setValue(&frameTime, node->first_attribute("frameTime"));
	for(xml_node<>* currNode = node->first_node("frame");
			currNode!=0;
			currNode = currNode->next_sibling("frame")){
		frame.push_back(new FrameType(currNode));
	}
}

AnimationType::~AnimationType(){
	for(vector<FrameType* >::iterator it = frame.begin();
			it!=frame.end();++it){
		delete *it;
	}
;}

void AnimationType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" cycled=\""<<cycled<<"\"";
		out<<" frameTime=\""<<frameTime<<"\"";
		out<<">\n";
	}
	for(auto iter:frame){
		iter->save("frame",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

AtlasParserRoot::AtlasParserRoot()
	:texture(0x0){}

AtlasParserRoot::AtlasParserRoot(AtlasParserRoot&& other)
	:texture(other.texture)
{
	other.texture=nullptr;
}
AtlasParserRoot::AtlasParserRoot(rapidxml::xml_node<>*  node)
	:texture(0x0){
		create(&texture, node->first_node("texture"));
}

AtlasParserRoot::~AtlasParserRoot(){
	delete texture;
}

void AtlasParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(texture!=0x0){
		texture->save("texture",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

FrameType::FrameType(){}

FrameType::FrameType(FrameType&& other)
	:x1(other.x1),y1(other.y1),x2(other.x2),y2(other.y2)
{
}
FrameType::FrameType(rapidxml::xml_node<>*  node){
	setValue(&x1, node->first_attribute("x1"));
	setValue(&y1, node->first_attribute("y1"));
	setValue(&x2, node->first_attribute("x2"));
	setValue(&y2, node->first_attribute("y2"));
}

FrameType::~FrameType(){
}

void FrameType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" x1=\""<<x1<<"\"";
		out<<" y1=\""<<y1<<"\"";
		out<<" x2=\""<<x2<<"\"";
		out<<" y2=\""<<y2<<"\"";
		out<<"/>";
	}
	out<<endl;
}

SpriteDescrType::SpriteDescrType()
	:frame(0x0),animation(0x0),name(""),initialAnimation(""),w(-1),h(-1),top(-1),left(-1),right(-1),bottom(-1){}

SpriteDescrType::SpriteDescrType(SpriteDescrType&& other)
	:frame(other.frame),animation(move(other.animation)),name(other.name),initialAnimation(other.initialAnimation),w(other.w),h(other.h),top(other.top),left(other.left),right(other.right),bottom(other.bottom)
{
	other.frame=nullptr;
}
SpriteDescrType::SpriteDescrType(rapidxml::xml_node<>*  node)
	:frame(0x0),animation(0x0),name(""),initialAnimation(""),w(-1),h(-1),top(-1),left(-1),right(-1),bottom(-1){
	setValue(&name, node->first_attribute("name"));
	setValue(&initialAnimation, node->first_attribute("initialAnimation"));
	setValue(&w, node->first_attribute("w"));
	setValue(&h, node->first_attribute("h"));
	setValue(&top, node->first_attribute("top"));
	setValue(&left, node->first_attribute("left"));
	setValue(&right, node->first_attribute("right"));
	setValue(&bottom, node->first_attribute("bottom"));
		create(&frame, node->first_node("frame"));
	for(xml_node<>* currNode = node->first_node("animation");
			currNode!=0;
			currNode = currNode->next_sibling("animation")){
		animation.push_back(new AnimationType(currNode));
	}
}

SpriteDescrType::~SpriteDescrType(){
	delete frame;
	for(vector<AnimationType* >::iterator it = animation.begin();
			it!=animation.end();++it){
		delete *it;
	}
;}

void SpriteDescrType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" initialAnimation=\""<<initialAnimation<<"\"";
		out<<" w=\""<<w<<"\"";
		out<<" h=\""<<h<<"\"";
		out<<" top=\""<<top<<"\"";
		out<<" left=\""<<left<<"\"";
		out<<" right=\""<<right<<"\"";
		out<<" bottom=\""<<bottom<<"\"";
		out<<">\n";
	}
	if(frame!=0x0){
		frame->save("frame",out,nestingLevel+1);
	}
	for(auto iter:animation){
		iter->save("animation",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

TextureType::TextureType()
	:sprite(0x0),file(""){}

TextureType::TextureType(TextureType&& other)
	:sprite(move(other.sprite)),file(other.file)
{
}
TextureType::TextureType(rapidxml::xml_node<>*  node)
	:sprite(0x0),file(""){
	setValue(&file, node->first_attribute("file"));
	for(xml_node<>* currNode = node->first_node("sprite");
			currNode!=0;
			currNode = currNode->next_sibling("sprite")){
		sprite.push_back(new SpriteDescrType(currNode));
	}
}

TextureType::~TextureType(){
	for(vector<SpriteDescrType* >::iterator it = sprite.begin();
			it!=sprite.end();++it){
		delete *it;
	}
;}

void TextureType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" file=\""<<file<<"\"";
		out<<">\n";
	}
	for(auto iter:sprite){
		iter->save("sprite",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

