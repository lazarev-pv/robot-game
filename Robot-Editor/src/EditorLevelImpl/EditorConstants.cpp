#include <EditorLevelImpl/EditorConstants.h>

using namespace game;
using namespace std;


const std::string EditorConstants::PRES2D = "pres2d";
const std::string EditorConstants::PRES3D = "pres3d";
const std::string EditorConstants::GEOM_CHAIN = "geomChain";
const std::string EditorConstants::GEOM_OBJECT = "geomObject";
const std::string EditorConstants::GEOM_FIXTURE = "geomFixture";
const std::string EditorConstants::GEOM_JOINT = "geomJoint";
const std::string EditorConstants::SPOT_LIGHT = "spotLight";
const std::string EditorConstants::DIR_LIGHT = "dirLight";
const std::string EditorConstants::FX_OBJECT = "fxObject";
const std::string EditorConstants::EVENT = "event";
const std::string EditorConstants::ENEMY = "enemy";
const std::string EditorConstants::POINT = "point";

std::vector<std::string> EditorConstants::getTypes(){
    return vector<string>({
            PRES2D,
            PRES3D,
            GEOM_CHAIN,
            GEOM_OBJECT,
            GEOM_JOINT,
            SPOT_LIGHT,
            DIR_LIGHT,
            FX_OBJECT,
            EVENT,
            ENEMY,
            POINT});
}

