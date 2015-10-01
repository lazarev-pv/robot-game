#ifndef __EDITOR_GRID_H_
#define __EDITOR_GRID_H_

#include <ChibiEngine/Render/Shaders.h>
#include <glm/glm.hpp>

namespace game{

class Grid{
public:
	Grid(int xCelsHC, int yCelsHC);

	void draw();
    void setZ(float z){
        this->pos.z=z;
    }

private:
    ShaderValues sv;
    glm::vec3 pos;
};

}



#endif
