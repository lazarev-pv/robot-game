#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <ChibiEngine/Platform/Platform.h>
#include <iostream>
#include <memory>
#include <ChibiEngine/Common/ClassUtils.h>

namespace game{

    class Texture : private UniqueClass {
    public:

        Texture(int w, int h);
        Texture(const std::string& file_name, const std::string& name);
        ~Texture();

        inline GLuint getUID() const;
        inline int getWidth() const;
        inline int getHeight() const;
        inline const std::string& getName() const;
    private:
        friend std::ostream& operator<<(std::ostream&, const Texture&);

        std::string name;
        GLuint texture;
        int w,h;
    };

    inline const std::string& Texture::getName() const{
        return name;
    }

    inline GLuint Texture::getUID() const {
        return texture;
    }

    inline int Texture::getWidth() const{
        return w;
    }

    inline int Texture::getHeight() const{
        return h;
    }

}

#endif /* TEXTURE_H_ */
