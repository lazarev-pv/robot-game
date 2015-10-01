#include <EditorLevelImpl/EnemyObject.h>

#include <ChibiEngine/XMLParser/WorldParser.h>
#include <EditorLevel.h>

using namespace game;
using namespace std;
using namespace glm;

EnemyObject::EnemyObject(const glm::vec2& pos, const std::string& enemyClass,const std::string& name)
    :NamedPointObject(vec3(pos,0),name,EditorConstants::ENEMY){
    setPointText(enemyClass);
}

EnemyObject::EnemyObject(EnemyType* pnt)
    :EnemyObject(vec2(pnt->pos->x,pnt->pos->y),pnt->enemyClass,pnt->name){
}

void EnemyObject::save2World(WorldType* world){
    EnemyType* et =  new EnemyType();
    et->name=getName();
    et->enemyClass = getPointText();
    et->pos=new Point2D();
    et->pos->x = getPosition().x;
    et->pos->y = getPosition().y;
    if(world->enemies==nullptr){
        world->enemies = new EnemyList();
    }
    world->enemies->enemy.push_back(et);
}

void EnemyObject::setSelected(bool isSelected){
    NamedPointObject::setSelected(isSelected);
    if(isSelected)
        EditorLevel::instance()->selectEnemy(this);
}

void EnemyObject::setName(const std::string& name){
    OperableObject::setName(name);
}

