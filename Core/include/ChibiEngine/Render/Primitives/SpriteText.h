
#ifndef SPRITETEXT_H_
#define SPRITETEXT_H_

#include <string>
#include <glm/glm.hpp>
#include <ChibiEngine/UserInterface/LookAndFeel.h>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Handles.h>
#include <ChibiEngine/Common/ClassUtils.h>

namespace game{

	class SpriteText : private Noncopyable {
	public:
		SpriteText(FontCHandle font, const std::string& text, int size);
		SpriteText(const FontLAF& laf, const std::string& text);

		void draw(const glm::vec3& pos, const Color& color, CameraCHandle camera = nullptr, bool shadow = false);

        void setText(const std::string& newText);
		const std::string& getText() const{
			return text;
		}


        static glm::vec2 toScreenSize(const glm::vec2& worldSize);

		float getScreenHWidth();
		float getScreenHHeight();

		float getWorldHW();
		float getWorldHH();

	private:
        //void recreateTextures();
        void genBuffers();
        void drawImpl(const glm::vec3& pos, const Color& color, CameraCHandle camera);
        ScreenFrameId getFrameId();

        ShaderValues sv;
		FontCHandle font;
		std::string text;

		float hw=0,hh=0;
		float screenHWidth=0, screenHHeight=0;
		int size;


	};

}



#endif /* SPRITETEXT_H_ */
