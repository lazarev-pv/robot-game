#ifndef __GRID_H_
#define __GRID_H_

#include <glm/glm.hpp>
#include <ChibiEngine/UserInterface/TextLabel.h>
#include <vector>

namespace game{

class PatternSprite;
class Grid{
public:
	Grid(float hw, float hh);
	~Grid();

	void draw();

private:
	struct Label{
		Label(SpriteText* sprite, float offset):
			sprite(sprite),offset(offset){};
		SpriteText* sprite;
		float offset;
	};

	PatternSprite* background;
	glm::vec2 backScale;
	std::vector<Label> labels;
	ShaderValues sv;
};

}



#endif
