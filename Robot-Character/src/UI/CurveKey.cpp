#include <UI/CurveKey.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>

using namespace game;
using namespace std;
using namespace glm;

void CurveKey::draw(const glm::vec2& begin, const glm::vec2& end, bool highlighted) const{
    Game::getPrimitiveDrawer()->drawLine(
            vec3(begin,0),
            vec3(end,0),
            highlighted?rgb(155, 155, 155):rgb(0, 156, 156));
}