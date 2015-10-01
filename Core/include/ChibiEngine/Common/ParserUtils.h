#ifndef __HEADER_PARSER_UTILS_
#define __HEADER_PARSER_UTILS_

#include <string>
#include <glm/glm.hpp>
#include "RAIIUtils.h"
#include <rapid/rapidxml.hpp>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Render/Color.h>

class Point3D;
class Point2D;
class ColorType;
namespace game{

    Color parse(ColorType *color, const Color& defaultColor = Color::Black);
    glm::vec3 parse(Point3D* point);
    glm::vec2 parse(Point2D* point);

    Point2D* create2D(const glm::vec2& pnt);
    Point2D* create2D(const glm::vec3& pnt);
    Point3D* create3D(const glm::vec2& pnt);
    Point3D* create3D(const glm::vec3& pnt);

    template<typename T>
    T createRoot(const std::string& fileName){
        custom_unique_ptr<char> buffer = Game::getFileSystem()->readTextFile(fileName);
        if(buffer){
            rapidxml::xml_document<> doc;
            doc.parse<0>(buffer.get());
            return T(&doc);
        }else{
            return T();
        }
    }
}

#endif

