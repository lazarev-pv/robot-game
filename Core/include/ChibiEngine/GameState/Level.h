
#ifndef LEVEL_H_
#define LEVEL_H_

#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Handles.h>

namespace game{

	class Level{
	public:
		virtual ~Level() = default;
		virtual void draw(CameraCHandle camera) = 0;
	};
}


#endif
