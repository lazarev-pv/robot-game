#ifndef SCREEN_H
#define	 SCREEN_H
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Resource/Texture.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/UserInterface/LookAndFeel.h>
#include <ChibiEngine/Render/RenderHandles.h>

namespace game{

    class TexturedObject;
	class LookAndFeel;
    class ScreenSystem{
    public:

        ScreenSystem(const ScreenType* screen);
        ~ScreenSystem();
 
        //todo set user interface part of game state
        void setLookAndFeel(LookAndFeel lookAndFeel);

        void resize(unsigned int width, unsigned int height);
        inline const LookAndFeel& getLookAndFeel() const;

        inline unsigned int getWidth() const { return width; }
        inline unsigned int getHeight() const { return height; }
        inline glm::vec2 getSize() const { return glm::vec2(getWidth(), getHeight()); }

        inline glm::vec2 getDensity() const { return glm::vec2(getWidthDensity(),getHeightDensity());}
        inline glm::vec2 getDensityNoAspect() const { return glm::vec2(getWidthDensity(), getHeightDensityNoAspect());}
        inline float getWidthDensity() const { return 2.0f/width; }
        inline float getHeightDensity() const { return aspectRatio*2.0f/height; }
        inline float getHeightDensityNoAspect() const { return 2.0f/height; }
        inline float getAspectRatio() const { return aspectRatio; }

        void Clear();

        inline const Logger& getLogger();

        /** Запрос на ленивое создание внеэкранного буфера
        * @pram size - полный размер буфера впикселях */
        ScreenFrameId generateFrameNumber(const glm::ivec2& size=glm::ivec2(-1,-1));
        const Texture& getFrameTexture(ScreenFrameId num);
        void bindFrame(ScreenFrameId num);
        void mainFrame();

        inline GLuint getIndexBuffer() const;
        inline GLuint getVertexBuffer() const;
        inline bool isEnabledDepthTest() const;
        inline bool isEnabledBlending() const;

    private:
        class Frame;
        const Frame* getFrame(ScreenFrameId num);
        struct FrameRequest{
            glm::ivec2 size;
            std::unique_ptr<const Frame> frame;
        };

        Logger logger;
        unsigned int width,height;
        LookAndFeel lookAndFeel;
        float aspectRatio;

        std::vector<FrameRequest> frames;
        ScreenFrameId frameNum;
        GLuint buffers[2];
        bool blendingEnabled, depthTestEnabled;
    };

    inline bool ScreenSystem::isEnabledDepthTest() const{
        return depthTestEnabled;
    }

    inline bool ScreenSystem::isEnabledBlending() const{
        return blendingEnabled;
    }

    inline const Logger& ScreenSystem::getLogger(){
        return logger;
    }

    inline GLuint ScreenSystem::getIndexBuffer() const{
        return buffers[0];
    }

    inline GLuint ScreenSystem::getVertexBuffer() const{
        return buffers[1];
    }

    inline const LookAndFeel& ScreenSystem::getLookAndFeel() const{
        return lookAndFeel;
    }


}
#endif	

