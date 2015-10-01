#ifndef __BATCH_TEXTURE_H_
#define __BATCH_TEXTURE_H_

#include <string>
#include <ChibiEngine/Resource/Texture.h>
#include <glm/glm.hpp>
#include <vector>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Handles.h>
#include <ChibiEngine/Common/ClassUtils.h>

namespace game{

	struct BatchElement{
		BatchElement(const glm::vec3& pos,
					const glm::vec2& scale,
					const glm::vec2& size,
					float angle,
					const glm::vec2& t1,
					const glm::vec2& t2,
					const glm::vec2& repeats,
                    float zOrder):
						pos(pos),
						scale(scale),
						size(size),
						angle(angle),
						t1(t1),
						t2(t2),
						repeats(repeats),
                        zOrder(zOrder){}

		glm::vec3 pos;
		glm::vec2 scale;
		glm::vec2 size;
		float angle;
		glm::vec2 t1;
		glm::vec2 t2;
		glm::vec2 repeats;
        int zOrder;
	};

	class Camera;
	class BatchTexture : private UniqueClass {
	public:
		//Создает объект с текстурой
		BatchTexture(TextureCHandle texture, std::vector<BatchElement>& elements);

		//Рисует изображения исходя из переданных координат
		void draw(CameraCHandle camera,const Color& ambient=Color::White) const;

	private:
        ShaderValues sv;
	};

}


#endif /* TEXTUREDOBJECT_H_ */
