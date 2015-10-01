#ifndef __STATIC_DECORATIONS_H_
#define __STATIC_DECORATIONS_H_

#include <LevelObjects/LevelObject.h>

#include <vector>

class ArtType;
namespace game{

	class BatchTexture;
	class StaticDecorations : public LevelObject{
	public:
		StaticDecorations(int uid, const std::vector<ArtType* >& data, float z, Layer* layer);

		virtual void draw(CameraCHandle camera,const Color& ambientLight);
		virtual glm::vec3 getPosition();
    protected:
        virtual ~StaticDecorations();
	private:
		std::vector<BatchTexture*> textures;
	};
}



#endif
