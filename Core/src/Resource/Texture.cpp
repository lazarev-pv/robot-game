#include <ChibiEngine/Resource/Texture.h>
#include <png.h>


#include <ChibiEngine/Game.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Resource/Resources.h>
using namespace game;

Texture::Texture(int w, int h)
	:w(w),h(h){

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h,0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Texture::Texture(const std::string& file_name, const std::string& name):
    name(name){
    if(name.size()==0){
        this->name=FileSystem::getPureName(name);
    }
	png_byte header[8];
	FILE *fp = fopen(Game::getFileSystem()->getFullName(file_name).c_str(), "rb");
	if(fp == 0){
		Game::getLogger().error("error: %s file not found.\n", file_name.c_str());
		return;
	}

	// read the header
	fread(header, 1, 8, fp);
	if(png_sig_cmp(header, 0, 8)){
		Game::getLogger().error("error: %s is not a PNG.\n", file_name.c_str());
		fclose(fp);
		return;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr){
		Game::getLogger().error("error: png_create_read_struct returned 0.\n");
		fclose(fp);
		return;
	}

	// create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr){
		Game::getLogger().error("error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return;
	}

	// create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info){
		Game::getLogger().error("error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return;
	}

	// the code in this if statement gets called if libpng encounters an error
	if (setjmp(png_jmpbuf(png_ptr))) {
		Game::getLogger().error("error from libpng\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return;
	}

	// init png reading
	png_init_io(png_ptr, fp);
	// let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);
	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);
	// variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 temp_width, temp_height;
	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
		NULL, NULL, NULL);

	w= temp_width;
	h= temp_height;

	if (bit_depth != 8){
		Game::getFileSystem()->getLogger().error("%s: Unsupported bit depth %d.  Must be 8.\n", file_name.c_str(), bit_depth);
		return;
	}

	GLint format;
	switch(color_type)
	{
	case PNG_COLOR_TYPE_RGB:
		format = GL_RGB;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		format = GL_RGBA;
		break;
	default:
		Game::getFileSystem()->getLogger().error("%s: Unknown libpng color type %d.\n", file_name.c_str(), color_type);
		return;
	}

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// glTexImage2d requires rows to be 4-byte aligned
	rowbytes += 3 - ((rowbytes-1) % 4);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte * image_data = (png_byte *)std::malloc(rowbytes * temp_height * sizeof(png_byte)+15);
	if (image_data == NULL)
	{
		Game::getFileSystem()->getLogger().error("error: could not allocate memory for PNG image data\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return;
	}

	// row_pointers is for pointing to image_data for reading the png with libpng
	png_byte ** row_pointers = (png_byte **)malloc(temp_height * sizeof(png_byte *));
	if (row_pointers == NULL){
		Game::getFileSystem()->getLogger().error("error: could not allocate memory for PNG row pointers\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		std::free(image_data);
		fclose(fp);
		return;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (unsigned int i = 0; i < temp_height; i++){
		row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
	}

	// read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	// Generate the OpenGL texture object
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
   // glGenerateMipmap(texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// clean up
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	std::free(image_data);
	free(row_pointers);
	fclose(fp);
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
}

namespace game{
	std::ostream& operator<<(std::ostream& str, const Texture& tex){
		return str<<"Texture:{id="<<tex.texture<<'}';
	}
}

