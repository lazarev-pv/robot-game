#ifndef __CHIBIENGINE_RENDER_COLOR_
#define __CHIBIENGINE_RENDER_COLOR_

#include <string>
#include <glm/glm.hpp>

class Point3D;
class ColorType;
namespace game{

    class Color{
    public:
        using Integral = unsigned int;

        static const Integral MAX_INT_VALUE = 255;
        static const Color White;
        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Magenta;
        static const Color Aqua;
        static const Color Orange;

        //ctor-s
        Color();
        Color(const std::string hex, float a=1.0f);
        Color(Integral r, Integral g, Integral b, Integral a);
        Color(float r, float g, float b, float a);
        Color(const glm::vec3& rgb);
        Color(const glm::vec4& rgba);

        //operators
        inline Color operator* (const Color& other) const;
        inline Color operator+ (const Color& other) const;
        inline Color operator* (float other) const;

        //Modifiers
        inline Color& setRed(float r);
        inline Color& setBlue(float b);
        inline Color& setGreen(float g);
        inline Color& setAlpha(float a);

        // Converters
        const glm::vec4& toVector() const;
        ColorType* toColorType() const;
        Point3D* toPoint3D() const;
        std::string toString() const;
        std::string toHex() const;

    private:
        glm::vec4 presentation;

    };

    inline Color rgba(Color::Integral r, Color::Integral g, Color::Integral b, Color::Integral a);
    inline Color rgb(Color::Integral r, Color::Integral g, Color::Integral b);
    Color parseFromToString(const std::string& toStringResult);

}

inline game::Color game::rgba(Color::Integral r, Color::Integral g, Color::Integral b, Color::Integral a){
    return Color(r,g,b,a);
}

inline game::Color game::rgb(Color::Integral r, Color::Integral g, Color::Integral b){
    return Color(r,g,b, game::Color::MAX_INT_VALUE);
}

inline game::Color& game::Color::setRed(float r){
    presentation.r=r;
    return *this;
}

inline game::Color& game::Color::setBlue(float b){
    presentation.b=b;
    return *this;
}

inline game::Color& game::Color::setGreen(float g){
    presentation.g=g;
    return *this;
}

inline game::Color& game::Color::setAlpha(float a){
    presentation.a=a;
    return *this;
}

inline game::Color game::Color::operator*(const game::Color& other) const{
    return Color(presentation*other.presentation);
}

inline game::Color game::Color::operator* (float other) const{
    return Color(presentation*other);
}

inline game::Color game::Color::operator+ (const Color& other) const{
    return Color(presentation+other.presentation);
}

#endif
