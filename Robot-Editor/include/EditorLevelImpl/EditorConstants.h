#ifndef __EDITOR_CONSTANTS_H_
#define __EDITOR_CONSTANTS_H_

#include <string>
#include <vector>

namespace game{

    class EditorConstants{
    public:
        static const std::string PRES2D;
        static const std::string PRES3D;
        static const std::string GEOM_CHAIN;
        static const std::string GEOM_FIXTURE;
        static const std::string GEOM_OBJECT;
        static const std::string GEOM_JOINT;
        static const std::string SPOT_LIGHT;
        static const std::string DIR_LIGHT;
        static const std::string FX_OBJECT;
        static const std::string EVENT;
        static const std::string ENEMY;
        static const std::string POINT;

        static std::vector<std::string> getTypes();

    };

}

#endif