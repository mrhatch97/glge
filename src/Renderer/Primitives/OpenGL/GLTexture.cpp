#include "GLTexture.h"

#include <glge/Util/Util.h>

#include <SOIL/SOIL.h>

namespace eng::render::primitive::opengl
{
	GLuint GLTexture::load_texture(const res::TextureData & info)
	{
		GLuint texture;
		// Create ID for texture
		glGenTextures(1, &texture);

		// Set this texture to be the one we are working with
		glBindTexture(GL_TEXTURE_2D, texture);

		// Generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, info.data.get());

		// TODO https://www.khronos.org/opengl/wiki/Common_Mistakes#Creating_a_complete_texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		// Set bi-linear filtering for both minification and magnification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (GLenum err; (err = glGetError()) != GL_NO_ERROR)
		{
			throw std::runtime_error(EXC_MSG("Failed to create texture"));
		}

		return texture;

	}

	GLuint GLTexture::load_texture_file(const res::TextureFileInfo & info)
	{
		GLuint texture = SOIL_load_OGL_texture(info.path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS);

		if (!texture)
		{
			czstring soil_err = SOIL_last_result();
			throw std::runtime_error(EXC_MSG("Failed to load texture file: " + info.path + "\nSOIL error: " + std::string(soil_err)));
		}

		return texture;
	}

	void GLTexture::activate() const
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	GLTexture::GLTexture(const res::TextureData & info) : id(load_texture(info))
	{
	}

	GLTexture::GLTexture(const res::TextureFileInfo & info) : id(load_texture_file(info))
	{
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &id);
	}
}