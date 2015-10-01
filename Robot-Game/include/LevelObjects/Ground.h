#ifndef __HEADER_GROUND_H_
#define __HEADER_GROUND_H_

#include <LevelObjects/PhysLevelObject.h>
#include <ChibiEngine/Render/Primitives/LinesChain.h>

class GeometrySlot;
namespace game{
	class Ground : public PhysLevelObject{
	public:
		Ground(int uid, GeometrySlot* slot, Layer* layer);

		virtual void draw(CameraCHandle camera,const Color& ambientLight);
    protected:
        virtual ~Ground();
	private:
		LinesChain chain;
	};


}


#endif
