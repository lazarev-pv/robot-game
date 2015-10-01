#include <ChibiEngine/XMLParser/WorldParser.h>
#include <cstring>
#include "util.h"
using namespace rapidxml;
using namespace std;
ArtType::ArtType()
	:image(0x0),scale(0x0),repeat(0x0),pos(0x0),color(0x0),rotation(0.0),name(""){}

ArtType::ArtType(ArtType&& other)
	:image(other.image),scale(other.scale),repeat(other.repeat),pos(other.pos),color(other.color),zOrder(other.zOrder),rotation(other.rotation),name(other.name)
{
	other.image=nullptr;
	other.scale=nullptr;
	other.repeat=nullptr;
	other.pos=nullptr;
	other.color=nullptr;
}
ArtType::ArtType(rapidxml::xml_node<>*  node)
	:image(0x0),scale(0x0),repeat(0x0),pos(0x0),color(0x0),rotation(0.0),name(""){
	setValue(&zOrder, node->first_attribute("zOrder"));
	setValue(&rotation, node->first_attribute("rotation"));
	setValue(&name, node->first_attribute("name"));
		create(&image, node->first_node("image"));
		create(&scale, node->first_node("scale"));
		create(&repeat, node->first_node("repeat"));
		create(&pos, node->first_node("pos"));
		create(&color, node->first_node("color"));
}

ArtType::~ArtType(){
	delete image;
	delete scale;
	delete repeat;
	delete pos;
	delete color;
}

void ArtType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" zOrder=\""<<zOrder<<"\"";
		out<<" rotation=\""<<rotation<<"\"";
		out<<" name=\""<<name<<"\"";
		out<<">\n";
	}
	if(image!=0x0){
		image->save("image",out,nestingLevel+1);
	}
	if(scale!=0x0){
		scale->save("scale",out,nestingLevel+1);
	}
	if(repeat!=0x0){
		repeat->save("repeat",out,nestingLevel+1);
	}
	if(pos!=0x0){
		pos->save("pos",out,nestingLevel+1);
	}
	if(color!=0x0){
		color->save("color",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

Box3DType::Box3DType()
	:pos(0x0),size(0x0),tex(0x0),color(0x0),texture(""),name(""){}

Box3DType::Box3DType(Box3DType&& other)
	:pos(other.pos),size(other.size),tex(other.tex),color(other.color),texture(other.texture),name(other.name)
{
	other.pos=nullptr;
	other.size=nullptr;
	other.tex=nullptr;
	other.color=nullptr;
}
Box3DType::Box3DType(rapidxml::xml_node<>*  node)
	:pos(0x0),size(0x0),tex(0x0),color(0x0),texture(""),name(""){
	setValue(&texture, node->first_attribute("texture"));
	setValue(&name, node->first_attribute("name"));
		create(&pos, node->first_node("pos"));
		create(&size, node->first_node("size"));
		create(&tex, node->first_node("tex"));
		create(&color, node->first_node("color"));
}

Box3DType::~Box3DType(){
	delete pos;
	delete size;
	delete tex;
	delete color;
}

void Box3DType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" texture=\""<<texture<<"\"";
		out<<" name=\""<<name<<"\"";
		out<<">\n";
	}
	if(pos!=0x0){
		pos->save("pos",out,nestingLevel+1);
	}
	if(size!=0x0){
		size->save("size",out,nestingLevel+1);
	}
	if(tex!=0x0){
		tex->save("tex",out,nestingLevel+1);
	}
	if(color!=0x0){
		color->save("color",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

DirLightType::DirLightType()
	:lt(0x0),rt(0x0),lb(0x0),rb(0x0){}

DirLightType::DirLightType(DirLightType&& other)
	:lt(other.lt),rt(other.rt),lb(other.lb),rb(other.rb)
{
	other.lt=nullptr;
	other.rt=nullptr;
	other.lb=nullptr;
	other.rb=nullptr;
}
DirLightType::DirLightType(rapidxml::xml_node<>*  node)
	:lt(0x0),rt(0x0),lb(0x0),rb(0x0){
		create(&lt, node->first_node("lt"));
		create(&rt, node->first_node("rt"));
		create(&lb, node->first_node("lb"));
		create(&rb, node->first_node("rb"));
}

DirLightType::~DirLightType(){
	delete lt;
	delete rt;
	delete lb;
	delete rb;
}

void DirLightType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(lt!=0x0){
		lt->save("lt",out,nestingLevel+1);
	}
	if(rt!=0x0){
		rt->save("rt",out,nestingLevel+1);
	}
	if(lb!=0x0){
		lb->save("lb",out,nestingLevel+1);
	}
	if(rb!=0x0){
		rb->save("rb",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ERBoxType::ERBoxType()
	:art(0x0),x(0),y(0),friction(1.0),density(1.0),sensor(false),category(""){}

ERBoxType::ERBoxType(ERBoxType&& other)
	:art(move(other.art)),x(other.x),y(other.y),hw(other.hw),hh(other.hh),friction(other.friction),density(other.density),sensor(other.sensor),category(other.category)
{
}
ERBoxType::ERBoxType(rapidxml::xml_node<>*  node)
	:art(0x0),x(0),y(0),friction(1.0),density(1.0),sensor(false),category(""){
	setValue(&x, node->first_attribute("x"));
	setValue(&y, node->first_attribute("y"));
	setValue(&hw, node->first_attribute("hw"));
	setValue(&hh, node->first_attribute("hh"));
	setValue(&friction, node->first_attribute("friction"));
	setValue(&density, node->first_attribute("density"));
	setValue(&sensor, node->first_attribute("sensor"));
	setValue(&category, node->first_attribute("category"));
	for(xml_node<>* currNode = node->first_node("art");
			currNode!=0;
			currNode = currNode->next_sibling("art")){
		art.push_back(new ArtType(currNode));
	}
}

ERBoxType::~ERBoxType(){
	for(vector<ArtType* >::iterator it = art.begin();
			it!=art.end();++it){
		delete *it;
	}
;}

void ERBoxType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" x=\""<<x<<"\"";
		out<<" y=\""<<y<<"\"";
		out<<" hw=\""<<hw<<"\"";
		out<<" hh=\""<<hh<<"\"";
		out<<" friction=\""<<friction<<"\"";
		out<<" density=\""<<density<<"\"";
		out<<" sensor=\""<<sensor<<"\"";
		out<<" category=\""<<category<<"\"";
		out<<">\n";
	}
	for(auto iter:art){
		iter->save("art",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ERChainType::ERChainType()
	:point(0x0),friction(1.0),category(""){}

ERChainType::ERChainType(ERChainType&& other)
	:point(move(other.point)),friction(other.friction),category(other.category)
{
}
ERChainType::ERChainType(rapidxml::xml_node<>*  node)
	:point(0x0),friction(1.0),category(""){
	setValue(&friction, node->first_attribute("friction"));
	setValue(&category, node->first_attribute("category"));
	for(xml_node<>* currNode = node->first_node("point");
			currNode!=0;
			currNode = currNode->next_sibling("point")){
		point.push_back(new Point2D(currNode));
	}
}

ERChainType::~ERChainType(){
	for(vector<Point2D* >::iterator it = point.begin();
			it!=point.end();++it){
		delete *it;
	}
;}

void ERChainType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" friction=\""<<friction<<"\"";
		out<<" category=\""<<category<<"\"";
		out<<">\n";
	}
	for(auto iter:point){
		iter->save("point",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

EnemyList::EnemyList()
	:enemy(0x0){}

EnemyList::EnemyList(EnemyList&& other)
	:enemy(move(other.enemy))
{
}
EnemyList::EnemyList(rapidxml::xml_node<>*  node)
	:enemy(0x0){
	for(xml_node<>* currNode = node->first_node("enemy");
			currNode!=0;
			currNode = currNode->next_sibling("enemy")){
		enemy.push_back(new EnemyType(currNode));
	}
}

EnemyList::~EnemyList(){
	for(vector<EnemyType* >::iterator it = enemy.begin();
			it!=enemy.end();++it){
		delete *it;
	}
;}

void EnemyList::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:enemy){
		iter->save("enemy",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

EnemyType::EnemyType()
	:pos(0x0),name(""),enemyClass(""){}

EnemyType::EnemyType(EnemyType&& other)
	:pos(other.pos),name(other.name),enemyClass(other.enemyClass)
{
	other.pos=nullptr;
}
EnemyType::EnemyType(rapidxml::xml_node<>*  node)
	:pos(0x0),name(""),enemyClass(""){
	setValue(&name, node->first_attribute("name"));
	setValue(&enemyClass, node->first_attribute("enemyClass"));
		create(&pos, node->first_node("pos"));
}

EnemyType::~EnemyType(){
	delete pos;
}

void EnemyType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" enemyClass=\""<<enemyClass<<"\"";
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

EventType::EventType()
	:pos(0x0),size(0x0),name(""),onEnter(""),onLeave(""),infinite(false){}

EventType::EventType(EventType&& other)
	:pos(other.pos),size(other.size),name(other.name),onEnter(other.onEnter),onLeave(other.onLeave),infinite(other.infinite)
{
	other.pos=nullptr;
	other.size=nullptr;
}
EventType::EventType(rapidxml::xml_node<>*  node)
	:pos(0x0),size(0x0),name(""),onEnter(""),onLeave(""),infinite(false){
	setValue(&name, node->first_attribute("name"));
	setValue(&onEnter, node->first_attribute("onEnter"));
	setValue(&onLeave, node->first_attribute("onLeave"));
	setValue(&infinite, node->first_attribute("infinite"));
		create(&pos, node->first_node("pos"));
		create(&size, node->first_node("size"));
}

EventType::~EventType(){
	delete pos;
	delete size;
}

void EventType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" onEnter=\""<<onEnter<<"\"";
		out<<" onLeave=\""<<onLeave<<"\"";
		out<<" infinite=\""<<infinite<<"\"";
		out<<">\n";
	}
	if(pos!=0x0){
		pos->save("pos",out,nestingLevel+1);
	}
	if(size!=0x0){
		size->save("size",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

FXListType::FXListType()
	:fx(0x0){}

FXListType::FXListType(FXListType&& other)
	:fx(move(other.fx))
{
}
FXListType::FXListType(rapidxml::xml_node<>*  node)
	:fx(0x0){
	for(xml_node<>* currNode = node->first_node("fx");
			currNode!=0;
			currNode = currNode->next_sibling("fx")){
		fx.push_back(new FXType(currNode));
	}
}

FXListType::~FXListType(){
	for(vector<FXType* >::iterator it = fx.begin();
			it!=fx.end();++it){
		delete *it;
	}
;}

void FXListType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:fx){
		iter->save("fx",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

FXType::FXType()
	:dir(0x0),color(0x0),name(""),effect(""){}

FXType::FXType(FXType&& other)
	:dir(other.dir),color(other.color),name(other.name),effect(other.effect),hw(other.hw),hh(other.hh),x(other.x),y(other.y),zOrder(other.zOrder)
{
	other.dir=nullptr;
	other.color=nullptr;
}
FXType::FXType(rapidxml::xml_node<>*  node)
	:dir(0x0),color(0x0),name(""),effect(""){
	setValue(&name, node->first_attribute("name"));
	setValue(&effect, node->first_attribute("effect"));
	setValue(&hw, node->first_attribute("hw"));
	setValue(&hh, node->first_attribute("hh"));
	setValue(&x, node->first_attribute("x"));
	setValue(&y, node->first_attribute("y"));
	setValue(&zOrder, node->first_attribute("zOrder"));
		create(&dir, node->first_node("dir"));
		create(&color, node->first_node("color"));
}

FXType::~FXType(){
	delete dir;
	delete color;
}

void FXType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" effect=\""<<effect<<"\"";
		out<<" hw=\""<<hw<<"\"";
		out<<" hh=\""<<hh<<"\"";
		out<<" x=\""<<x<<"\"";
		out<<" y=\""<<y<<"\"";
		out<<" zOrder=\""<<zOrder<<"\"";
		out<<">\n";
	}
	if(dir!=0x0){
		dir->save("dir",out,nestingLevel+1);
	}
	if(color!=0x0){
		color->save("color",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

GeometryList::GeometryList()
	:slot(0x0),join(0x0){}

GeometryList::GeometryList(GeometryList&& other)
	:slot(move(other.slot)),join(move(other.join))
{
}
GeometryList::GeometryList(rapidxml::xml_node<>*  node)
	:slot(0x0),join(0x0){
	for(xml_node<>* currNode = node->first_node("slot");
			currNode!=0;
			currNode = currNode->next_sibling("slot")){
		slot.push_back(new GeometrySlot(currNode));
	}
	for(xml_node<>* currNode = node->first_node("join");
			currNode!=0;
			currNode = currNode->next_sibling("join")){
		join.push_back(new JointType(currNode));
	}
}

GeometryList::~GeometryList(){
	for(vector<GeometrySlot* >::iterator it = slot.begin();
			it!=slot.end();++it){
		delete *it;
	}
;	for(vector<JointType* >::iterator it = join.begin();
			it!=join.end();++it){
		delete *it;
	}
;}

void GeometryList::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:slot){
		iter->save("slot",out,nestingLevel+1);
	}
	for(auto iter:join){
		iter->save("join",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

GeometrySlot::GeometrySlot()
	:box(0x0),chain(0x0),name(""),physType(2),x(0),y(0){}

GeometrySlot::GeometrySlot(GeometrySlot&& other)
	:box(move(other.box)),chain(other.chain),name(other.name),physType(other.physType),x(other.x),y(other.y)
{
	other.chain=nullptr;
}
GeometrySlot::GeometrySlot(rapidxml::xml_node<>*  node)
	:box(0x0),chain(0x0),name(""),physType(2),x(0),y(0){
	setValue(&name, node->first_attribute("name"));
	setValue(&physType, node->first_attribute("physType"));
	setValue(&x, node->first_attribute("x"));
	setValue(&y, node->first_attribute("y"));
	for(xml_node<>* currNode = node->first_node("box");
			currNode!=0;
			currNode = currNode->next_sibling("box")){
		box.push_back(new ERBoxType(currNode));
	}
		create(&chain, node->first_node("chain"));
}

GeometrySlot::~GeometrySlot(){
	for(vector<ERBoxType* >::iterator it = box.begin();
			it!=box.end();++it){
		delete *it;
	}
;	delete chain;
}

void GeometrySlot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" physType=\""<<physType<<"\"";
		out<<" x=\""<<x<<"\"";
		out<<" y=\""<<y<<"\"";
		out<<">\n";
	}
	for(auto iter:box){
		iter->save("box",out,nestingLevel+1);
	}
	if(chain!=0x0){
		chain->save("chain",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

JointType::JointType()
	:offset1(0x0),offset2(0x0),name(""),obj1Name(""),obj2Name(""){}

JointType::JointType(JointType&& other)
	:offset1(other.offset1),offset2(other.offset2),name(other.name),obj1Name(other.obj1Name),obj2Name(other.obj2Name),frequence(other.frequence),damp(other.damp)
{
	other.offset1=nullptr;
	other.offset2=nullptr;
}
JointType::JointType(rapidxml::xml_node<>*  node)
	:offset1(0x0),offset2(0x0),name(""),obj1Name(""),obj2Name(""){
	setValue(&name, node->first_attribute("name"));
	setValue(&obj1Name, node->first_attribute("obj1Name"));
	setValue(&obj2Name, node->first_attribute("obj2Name"));
	setValue(&frequence, node->first_attribute("frequence"));
	setValue(&damp, node->first_attribute("damp"));
		create(&offset1, node->first_node("offset1"));
		create(&offset2, node->first_node("offset2"));
}

JointType::~JointType(){
	delete offset1;
	delete offset2;
}

void JointType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" obj1Name=\""<<obj1Name<<"\"";
		out<<" obj2Name=\""<<obj2Name<<"\"";
		out<<" frequence=\""<<frequence<<"\"";
		out<<" damp=\""<<damp<<"\"";
		out<<">\n";
	}
	if(offset1!=0x0){
		offset1->save("offset1",out,nestingLevel+1);
	}
	if(offset2!=0x0){
		offset2->save("offset2",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

LevelSettings::LevelSettings()
	:scriptName(""),startEvent(""){}

LevelSettings::LevelSettings(LevelSettings&& other)
	:scriptName(other.scriptName),startEvent(other.startEvent)
{
}
LevelSettings::LevelSettings(rapidxml::xml_node<>*  node)
	:scriptName(""),startEvent(""){
	setValue(&scriptName, node->first_attribute("scriptName"));
	setValue(&startEvent, node->first_attribute("startEvent"));
}

LevelSettings::~LevelSettings(){
}

void LevelSettings::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" scriptName=\""<<scriptName<<"\"";
		out<<" startEvent=\""<<startEvent<<"\"";
		out<<"/>";
	}
	out<<endl;
}

LightListType::LightListType()
	:light(0x0){}

LightListType::LightListType(LightListType&& other)
	:light(move(other.light))
{
}
LightListType::LightListType(rapidxml::xml_node<>*  node)
	:light(0x0){
	for(xml_node<>* currNode = node->first_node("light");
			currNode!=0;
			currNode = currNode->next_sibling("light")){
		light.push_back(new LightType(currNode));
	}
}

LightListType::~LightListType(){
	for(vector<LightType* >::iterator it = light.begin();
			it!=light.end();++it){
		delete *it;
	}
;}

void LightListType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:light){
		iter->save("light",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

LightType::LightType()
	:name(""),enable(true){}

LightTypeUnion::LightTypeUnion()
	:spot(0x0),directional(0x0){}

LightType::LightType(LightType&& other)
	:choiceValue(move(other.choiceValue)),name(other.name),r(other.r),g(other.g),b(other.b),a(other.a),enable(other.enable){
}
LightTypeUnion::LightTypeUnion(LightTypeUnion&& other)
	:spot(other.spot),directional(other.directional)
{
	other.spot=nullptr;
	other.directional=nullptr;
}
LightType::LightType(rapidxml::xml_node<>*  node)
	:name(""),enable(true){
	setValue(&name, node->first_attribute("name"));
	setValue(&r, node->first_attribute("r"));
	setValue(&g, node->first_attribute("g"));
	setValue(&b, node->first_attribute("b"));
	setValue(&a, node->first_attribute("a"));
	setValue(&enable, node->first_attribute("enable"));
	xml_node<>* choiceNode = node->first_node();
	if(choiceNode!=0){
		if(strcmp(choiceNode->name(),"spot")==0){
			create(&choiceValue.spot, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"directional")==0){
			create(&choiceValue.directional, choiceNode);
		}
	}
}
LightType::~LightType(){
	delete choiceValue.spot;
	delete choiceValue.directional;
}

void LightType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" r=\""<<r<<"\"";
		out<<" g=\""<<g<<"\"";
		out<<" b=\""<<b<<"\"";
		out<<" a=\""<<a<<"\"";
		out<<" enable=\""<<enable<<"\"";
		out<<">\n";
	}
	if(choiceValue.spot!=0x0){
		choiceValue.spot->save("spot",out,nestingLevel+1);
	}
	if(choiceValue.directional!=0x0){
		choiceValue.directional->save("directional",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

NamedPoint2D::NamedPoint2D()
	:name(""){}

NamedPoint2D::NamedPoint2D(NamedPoint2D&& other)
	:x(other.x),y(other.y),name(other.name)
{
}
NamedPoint2D::NamedPoint2D(rapidxml::xml_node<>*  node)
	:name(""){
	setValue(&x, node->first_attribute("x"));
	setValue(&y, node->first_attribute("y"));
	setValue(&name, node->first_attribute("name"));
}

NamedPoint2D::~NamedPoint2D(){
}

void NamedPoint2D::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" x=\""<<x<<"\"";
		out<<" y=\""<<y<<"\"";
		out<<" name=\""<<name<<"\"";
		out<<"/>";
	}
	out<<endl;
}

Obj3DType::Obj3DType()
	:box3D(0x0){}

Obj3DType::Obj3DType(Obj3DType&& other)
	:box3D(move(other.box3D))
{
}
Obj3DType::Obj3DType(rapidxml::xml_node<>*  node)
	:box3D(0x0){
	for(xml_node<>* currNode = node->first_node("box3D");
			currNode!=0;
			currNode = currNode->next_sibling("box3D")){
		box3D.push_back(new Box3DType(currNode));
	}
}

Obj3DType::~Obj3DType(){
	for(vector<Box3DType* >::iterator it = box3D.begin();
			it!=box3D.end();++it){
		delete *it;
	}
;}

void Obj3DType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:box3D){
		iter->save("box3D",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

Reference::Reference()
	:name(""){}

Reference::Reference(Reference&& other)
	:name(other.name)
{
}
Reference::Reference(rapidxml::xml_node<>*  node)
	:name(""){
	setValue(&name, node->first_attribute("name"));
}

Reference::~Reference(){
}

void Reference::save(std::string tagName, std::ostream& out,int nestingLevel){
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

RobotCameraConstraints::RobotCameraConstraints(){}

RobotCameraConstraints::RobotCameraConstraints(RobotCameraConstraints&& other)
	:left(other.left),right(other.right),top(other.top),bottom(other.bottom)
{
}
RobotCameraConstraints::RobotCameraConstraints(rapidxml::xml_node<>*  node){
	setValue(&left, node->first_attribute("left"));
	setValue(&right, node->first_attribute("right"));
	setValue(&top, node->first_attribute("top"));
	setValue(&bottom, node->first_attribute("bottom"));
}

RobotCameraConstraints::~RobotCameraConstraints(){
}

void RobotCameraConstraints::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" left=\""<<left<<"\"";
		out<<" right=\""<<right<<"\"";
		out<<" top=\""<<top<<"\"";
		out<<" bottom=\""<<bottom<<"\"";
		out<<"/>";
	}
	out<<endl;
}

RobotCameraSettings::RobotCameraSettings()
	:initPos(0x0),constraints(0x0){}

RobotCameraSettings::RobotCameraSettings(RobotCameraSettings&& other)
	:initPos(other.initPos),constraints(other.constraints)
{
	other.initPos=nullptr;
	other.constraints=nullptr;
}
RobotCameraSettings::RobotCameraSettings(rapidxml::xml_node<>*  node)
	:initPos(0x0),constraints(0x0){
		create(&initPos, node->first_node("initPos"));
		create(&constraints, node->first_node("constraints"));
}

RobotCameraSettings::~RobotCameraSettings(){
	delete initPos;
	delete constraints;
}

void RobotCameraSettings::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(initPos!=0x0){
		initPos->save("initPos",out,nestingLevel+1);
	}
	if(constraints!=0x0){
		constraints->save("constraints",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

SpotLightType::SpotLightType()
	:pos(0x0){}

SpotLightType::SpotLightType(SpotLightType&& other)
	:pos(other.pos),i(other.i),hw(other.hw),hh(other.hh)
{
	other.pos=nullptr;
}
SpotLightType::SpotLightType(rapidxml::xml_node<>*  node)
	:pos(0x0){
	setValue(&i, node->first_attribute("i"));
	setValue(&hw, node->first_attribute("hw"));
	setValue(&hh, node->first_attribute("hh"));
		create(&pos, node->first_node("pos"));
}

SpotLightType::~SpotLightType(){
	delete pos;
}

void SpotLightType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" i=\""<<i<<"\"";
		out<<" hw=\""<<hw<<"\"";
		out<<" hh=\""<<hh<<"\"";
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

WeatherType::WeatherType()
	:dayLight(0x0),nightLight(0x0),thunder(false){}

WeatherType::WeatherType(WeatherType&& other)
	:dayLight(other.dayLight),nightLight(other.nightLight),daytime(other.daytime),thunder(other.thunder)
{
	other.dayLight=nullptr;
	other.nightLight=nullptr;
}
WeatherType::WeatherType(rapidxml::xml_node<>*  node)
	:dayLight(0x0),nightLight(0x0),thunder(false){
	setValue(&daytime, node->first_attribute("daytime"));
	setValue(&thunder, node->first_attribute("thunder"));
		create(&dayLight, node->first_node("dayLight"));
		create(&nightLight, node->first_node("nightLight"));
}

WeatherType::~WeatherType(){
	delete dayLight;
	delete nightLight;
}

void WeatherType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" daytime=\""<<daytime<<"\"";
		out<<" thunder=\""<<thunder<<"\"";
		out<<">\n";
	}
	if(dayLight!=0x0){
		dayLight->save("dayLight",out,nestingLevel+1);
	}
	if(nightLight!=0x0){
		nightLight->save("nightLight",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

WorldLayers::WorldLayers()
	:art(0x0),objs3d(0x0),geometry(0x0),fxes(0x0),light(0x0),name(""){}

WorldLayers::WorldLayers(WorldLayers&& other)
	:art(move(other.art)),objs3d(other.objs3d),geometry(other.geometry),fxes(other.fxes),light(other.light),z(other.z),name(other.name)
{
	other.objs3d=nullptr;
	other.geometry=nullptr;
	other.fxes=nullptr;
	other.light=nullptr;
}
WorldLayers::WorldLayers(rapidxml::xml_node<>*  node)
	:art(0x0),objs3d(0x0),geometry(0x0),fxes(0x0),light(0x0),name(""){
	setValue(&z, node->first_attribute("z"));
	setValue(&name, node->first_attribute("name"));
	for(xml_node<>* currNode = node->first_node("art");
			currNode!=0;
			currNode = currNode->next_sibling("art")){
		art.push_back(new ArtType(currNode));
	}
		create(&objs3d, node->first_node("objs3d"));
		create(&geometry, node->first_node("geometry"));
		create(&fxes, node->first_node("fxes"));
		create(&light, node->first_node("light"));
}

WorldLayers::~WorldLayers(){
	for(vector<ArtType* >::iterator it = art.begin();
			it!=art.end();++it){
		delete *it;
	}
;	delete objs3d;
	delete geometry;
	delete fxes;
	delete light;
}

void WorldLayers::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" z=\""<<z<<"\"";
		out<<" name=\""<<name<<"\"";
		out<<">\n";
	}
	for(auto iter:art){
		iter->save("art",out,nestingLevel+1);
	}
	if(objs3d!=0x0){
		objs3d->save("objs3d",out,nestingLevel+1);
	}
	if(geometry!=0x0){
		geometry->save("geometry",out,nestingLevel+1);
	}
	if(fxes!=0x0){
		fxes->save("fxes",out,nestingLevel+1);
	}
	if(light!=0x0){
		light->save("light",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

WorldParserRoot::WorldParserRoot()
	:world(0x0){}

WorldParserRoot::WorldParserRoot(WorldParserRoot&& other)
	:world(other.world)
{
	other.world=nullptr;
}
WorldParserRoot::WorldParserRoot(rapidxml::xml_node<>*  node)
	:world(0x0){
		create(&world, node->first_node("world"));
}

WorldParserRoot::~WorldParserRoot(){
	delete world;
}

void WorldParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(world!=0x0){
		world->save("world",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

WorldType::WorldType()
	:resources(0x0),levelSettings(0x0),camera(0x0),points(0x0),layer(0x0),enemies(0x0),weather(0x0),event(0x0){}

WorldType::WorldType(WorldType&& other)
	:resources(other.resources),levelSettings(other.levelSettings),camera(other.camera),points(move(other.points)),layer(move(other.layer)),enemies(other.enemies),weather(other.weather),event(move(other.event))
{
	other.resources=nullptr;
	other.levelSettings=nullptr;
	other.camera=nullptr;
	other.enemies=nullptr;
	other.weather=nullptr;
}
WorldType::WorldType(rapidxml::xml_node<>*  node)
	:resources(0x0),levelSettings(0x0),camera(0x0),points(0x0),layer(0x0),enemies(0x0),weather(0x0),event(0x0){
		create(&resources, node->first_node("resources"));
		create(&levelSettings, node->first_node("levelSettings"));
		create(&camera, node->first_node("camera"));
	for(xml_node<>* currNode = node->first_node("points");
			currNode!=0;
			currNode = currNode->next_sibling("points")){
		points.push_back(new NamedPoint2D(currNode));
	}
	for(xml_node<>* currNode = node->first_node("layer");
			currNode!=0;
			currNode = currNode->next_sibling("layer")){
		layer.push_back(new WorldLayers(currNode));
	}
		create(&enemies, node->first_node("enemies"));
		create(&weather, node->first_node("weather"));
	for(xml_node<>* currNode = node->first_node("event");
			currNode!=0;
			currNode = currNode->next_sibling("event")){
		event.push_back(new EventType(currNode));
	}
}

WorldType::~WorldType(){
	delete resources;
	delete levelSettings;
	delete camera;
	for(vector<NamedPoint2D* >::iterator it = points.begin();
			it!=points.end();++it){
		delete *it;
	}
;	for(vector<WorldLayers* >::iterator it = layer.begin();
			it!=layer.end();++it){
		delete *it;
	}
;	delete enemies;
	delete weather;
	for(vector<EventType* >::iterator it = event.begin();
			it!=event.end();++it){
		delete *it;
	}
;}

void WorldType::save(std::string tagName, std::ostream& out,int nestingLevel){
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
	if(levelSettings!=0x0){
		levelSettings->save("levelSettings",out,nestingLevel+1);
	}
	if(camera!=0x0){
		camera->save("camera",out,nestingLevel+1);
	}
	for(auto iter:points){
		iter->save("points",out,nestingLevel+1);
	}
	for(auto iter:layer){
		iter->save("layer",out,nestingLevel+1);
	}
	if(enemies!=0x0){
		enemies->save("enemies",out,nestingLevel+1);
	}
	if(weather!=0x0){
		weather->save("weather",out,nestingLevel+1);
	}
	for(auto iter:event){
		iter->save("event",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

