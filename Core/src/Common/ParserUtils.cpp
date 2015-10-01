#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/Common/StringUtils.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Resource/FileSystem.h>

using namespace game;
using namespace glm;
using namespace std;


Color game::parse(ColorType *color, const Color& defaultColor){
    if(color == nullptr)
        return defaultColor;

    if(color->hex.size()==6){
        return Color(color->hex,color->a);
    }

    return Color(color->r,color->g, color->b, color->a);
}

glm::vec3 game::parse(Point3D* point){
    if(point == nullptr)
        return vec3(0,0,0);
    return vec3(point->x,point->y,point->z);
}

glm::vec2 game::parse(Point2D* point){
    if(point == nullptr)
        return vec2(0,0);
    return vec2(point->x, point->y);
}

Point2D* game::create2D(const glm::vec2& pnt){
    Point2D * p = new Point2D();
    p->x = pnt.x;
    p->y = pnt.y;
    return p;
}

Point2D* game::create2D(const glm::vec3& pnt){
    Point2D * p = new Point2D();
    p->x = pnt.x;
    p->y = pnt.y;
    return p;
}

Point3D* game::create3D(const glm::vec2& pnt){
    Point3D * p = new Point3D();
    p->x = pnt.x;
    p->y = pnt.y;
    p->z = 0;
    return p;
}

Point3D* game::create3D(const glm::vec3& pnt){
    Point3D * p = new Point3D();
    p->x = pnt.x;
    p->y = pnt.y;
    p->z = pnt.z;
    return p;
}