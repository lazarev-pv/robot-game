#include <LevelObjects/Ground.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/XMLParser/WorldParser.h>

using namespace std;
using namespace game;
using namespace glm;

Ground::Ground(int uid, GeometrySlot* slot, Layer* layer)
		:PhysLevelObject(uid,slot,layer),
		chain(Color::White){

		vector<vec3> pnts;
		if(slot->chain!=nullptr){
			for(auto pnt :slot->chain->point){
				pnts.push_back(vec3(pnt->x,pnt->y,0));
			}
		}
		chain.add(pnts);
}

Ground::~Ground(){
}

void Ground::draw(CameraCHandle camera,const Color& ambientLight){
	if(isDrawDebug())
		chain.draw(camera);
}
