#include <ChibiEngine/Resource/Font.h>
#include <ChibiEngine/Platform/Platform.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Render/ScreenSystem.h>

//debug
#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#include <unordered_map>

using namespace game;
using namespace glm;
using namespace std;

static constexpr int MAX_WIDTH = 2048;

GlythData::GlythData(FT_GlyphSlot g, glm::vec2 offset):
	advance(g->advance.x>>6, g->advance.y>>6),
	bitmapSize(g->bitmap.width,g->bitmap.rows/*+1*/),
	topLeftPnt(g->bitmap_left,g->bitmap_top),
	offset(offset) {
}

glm::ivec2 AtlasProperties::getAtlasSize() const{
	return glm::ivec2(w,h);
}

Font::~Font(){
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	for(auto& iter:atlas){
		glDeleteTextures(1, &iter.second.texUid);
	}
}

Font::Font(const std::string& font_file){

	Game::getFileSystem()->getLogger().debug("Loading font %s", font_file.c_str());
	int error = FT_Init_FreeType(&library); //todo Тяжеловесная штука сделать одной для всех шрифтов
	if(error){
		Game::getFileSystem()->getLogger().error("cannot create library. error: %d", error);
	}

	error = FT_New_Face(library, Game::getFileSystem()->getFullName(font_file).c_str(),0,&face);
	if(error == FT_Err_Unknown_File_Format){
		Game::getFileSystem()->getLogger().error("unknown font. error: %d", error);
	}else if(error){
		Game::getFileSystem()->getLogger().error("cannot load font. error: %d", error);
	}

	Game::getFileSystem()->getLogger().trace("Loading font - ok");

}

namespace game{
	std::ostream& operator<<(std::ostream& str, const Font& fnt){
		return str<<"Font:{name="<<fnt.face->family_name
		  /* <<";size="<<fnt.glyth_width<<','<<fnt.glyth_height<<'}'*/;
	}
}


AtlasProperties Font::createAtlas(float size) const{
	AtlasProperties atlasProperties;

    int error = FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(size));

	/*int error = FT_Set_Char_Size(face, 0, static_cast<FT_F26Dot6>(size*64),
			Game::getScreen()->getWidth(), Game::getScreen()->getHeight());*/
	if(error){
		Game::getFileSystem()->getLogger().error("changing font size error: %d", error);
		return atlasProperties;
	}

	// calc size
	FT_GlyphSlot g = face->glyph;
	atlasProperties.w = 0;
	atlasProperties.h = 0;
	int roww = 0;
	int rowh = 0;
	for(FT_ULong i = 32; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT )) {
			Game::getFileSystem()->getLogger().error("Loading character %c failed!\n", i);
			continue;
		}

		if (roww + g->bitmap.width + 1 >= MAX_WIDTH) {
			atlasProperties.w = std::max(atlasProperties.w, roww);
			atlasProperties.h += rowh;
			roww = 0;
			rowh = 0;
		}
		roww += g->bitmap.width + 1;
		rowh = std::max(rowh, static_cast<int>(g->bitmap.rows/*+1*/));
	}

	atlasProperties.w = std::max(atlasProperties.w, roww);
	atlasProperties.h += rowh;

	// create texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &atlasProperties.texUid);
	glBindTexture(GL_TEXTURE_2D, atlasProperties.texUid);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlasProperties.w, atlasProperties.h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int ox = 0;
	int oy = 0;
	rowh = 0;


	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}

		if (ox + g->bitmap.width + 1 >= MAX_WIDTH) {
			oy += rowh;
			rowh = 0;
			ox = 0;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		atlasProperties.glyths.push_back(GlythData(g,
				vec2(static_cast<float>(ox)/atlasProperties.w,
					 static_cast<float>(oy)/atlasProperties.h)));
		rowh = std::max(rowh, static_cast<int>(g->bitmap.rows/*+1*/));
		ox += g->bitmap.width + 1;
	}

	return atlasProperties;
}

AtlasProperties Font::getAtlas(int size) const{
	AtlasReference::iterator iter = atlas.find(size);
	if(iter!=atlas.end()){
		return iter->second;
	}

	AtlasProperties props = createAtlas(size);
	atlas.insert({size,props});
	return props;
}