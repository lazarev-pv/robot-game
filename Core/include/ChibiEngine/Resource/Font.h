
#ifndef FONT_H_
#define FONT_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include <ChibiEngine/Platform/Platform.h>
#include <ChibiEngine/Common/ClassUtils.h>
#include <ChibiEngine/Handles.h>
#include <ChibiEngine/Render/Shaders.h>
#include <unordered_map>

namespace game{

	struct GlythData{
		GlythData(FT_GlyphSlot glyth, glm::vec2 offset);

		glm::vec2 advance;
		glm::vec2 bitmapSize;
		glm::vec2 topLeftPnt;
		glm::vec2 offset;

	};

	struct AtlasProperties{
		GLuint texUid = 0;
		float maxH = -1;

		glm::ivec2 getAtlasSize() const;
		inline const GlythData& getGlythData(char ch) const;
	private:
		friend class Font;
		std::vector<GlythData> glyths;
		int w,h;
	};

	class Font : private UniqueClass {
	public:
		Font(const std::string& font_file);
		~Font();

		AtlasProperties getAtlas(int size) const;

	private:
		AtlasProperties createAtlas(float size) const;
		friend std::ostream& operator<<(std::ostream&, const Font&);

		using AtlasReference = std::unordered_map<int,AtlasProperties>;

		FT_Library library;
		FT_Face face;
		mutable AtlasReference atlas; // Мутабл потому как создается лениво
	};

	inline const GlythData& AtlasProperties::getGlythData(char ch) const{
		return glyths[static_cast<int>(ch)-32];
	}

}

#endif /* FONT_H_ */
