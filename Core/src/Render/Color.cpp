#include <ChibiEngine/Render/Color.h>
#include <sstream>
#include <ChibiEngine/Common/StringUtils.h>
#include <ChibiEngine/XMLParser/CommonParser.h>

using namespace game;
using namespace glm;
using namespace std;

const Color Color::White(1.0f,1.0f,1.0f,1.0f);
const Color Color::Black(0.0f,0.0f,0.0f,1.0f);
const Color Color::Red(1.0f,0.0f,0.0f,1.0f);
const Color Color::Green(0.0f,1.0f,0.0f,1.0f);
const Color Color::Blue(0.0f,0.0f,1.0f,1.0f);
const Color Color::Yellow(1.0f,1.0f,0.0f,1.0f);
const Color Color::Magenta(1.0f,0.0f,1.0f,1.0f);
const Color Color::Aqua(0.0f,1.0f,1.0f,1.0f);

const Color Color::Orange("ffa500");

Color::Color():
    presentation(0,0,0,1.0f){
}

Color::Color(const std::string hex, float a){
    assert(hex.size() == 6);
    unsigned int num = stoul(hex, 0, 16);

    presentation.x = ((num >> 16) & 0xff) / 255.0f;
    presentation.y = ((num >> 8) & 0xff) / 255.0f;
    presentation.z = ((num) & 0xff) / 255.0f;
    presentation.w = a;
}

Color::Color(Integral r, Integral g, Integral b, Integral a)
    :presentation(
        r/static_cast<float>(MAX_INT_VALUE),
        g/static_cast<float>(MAX_INT_VALUE),
        b/static_cast<float>(MAX_INT_VALUE),
        a/static_cast<float>(MAX_INT_VALUE)){
}

Color::Color(float r, float g, float b, float a)
    :presentation(r,g,b,a){
}

Color::Color(const glm::vec3& rgb)
    :presentation(rgb,1.0f){
}

Color::Color(const glm::vec4& rgba)
        :presentation(rgba){
}

const glm::vec4& Color::toVector() const{
    return presentation;
}

Point3D* Color::toPoint3D() const{
    Point3D* p =new Point3D();
    p->x=presentation.r;
    p->y=presentation.g;
    p->z=presentation.b;
    return p;
}

ColorType* Color::toColorType() const{
    ColorType* ct = new ColorType();
    ct->r=presentation.r;
    ct->g=presentation.g;
    ct->b=presentation.b;
    ct->a=presentation.a;
    return ct;
}

std::string Color::toString() const{
    ostringstream os;
    os<<presentation.r<<";"<<presentation.g<<";"<<presentation.b<<";"<<presentation.a;
    return os.str();
}

std::string Color::toHex() const{
    unsigned int r = static_cast<unsigned int>(presentation.r*255.0f);
    unsigned int g = static_cast<unsigned int>(presentation.g*255.0f);
    unsigned int b = static_cast<unsigned int>(presentation.b*255.0f);
    char hexcol[16];
    snprintf(hexcol, sizeof hexcol, "%02x%02x%02x", r, g, b);
    return string(hexcol);
}

Color game::parseFromToString(const std::string& toStringResult){
    vector<float> comps = extractFloats(toStringResult,";");
    if(comps.size()==4) {
        return Color(comps[0], comps[1], comps[2], comps[3]);
    }else{
        return Color::Black;
    }
}