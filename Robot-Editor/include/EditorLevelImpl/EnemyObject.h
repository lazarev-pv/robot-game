#ifndef __ENEMYOBJECT_H_
#define __ENEMYOBJECT_H_

#include <EditorLevelImpl/NamedPointObject.h>

class EnemyType;
namespace game{

class EnemyObject : public NamedPointObject{
public:
	EnemyObject(const glm::vec2& pos, const std::string& enemyClass,const std::string& name="");
	EnemyObject(EnemyType* pnt);
	virtual ~EnemyObject(){};

    virtual void setName(const std::string& name);
	virtual void save2World(WorldType* world);
    virtual void setSelected(bool isSelected);

};


}



#endif
