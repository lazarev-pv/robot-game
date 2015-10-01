#include <LevelObjects/Interactive.h>
#include <LevelObjects/LevelObject.h>
#include <Layer.h>

using namespace std;
using namespace game;
using namespace glm;

inline void LevelObject::requestDelete(bool instant){
    if(interact){
        interact->getLayer()->deleteObject(interact);
    }
    delete this;
}

void LevelObject::link2Interact(Interactive* interact){
    LevelObject::interact = interact;
}

Interactive* LevelObject::getInteract(){
    return interact;
}