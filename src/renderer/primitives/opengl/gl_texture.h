#pragma once

#include <SOIL/SOIL.h>

#include "gl_common.h"
#include <glge/renderer/primitives/primitive_data.h>

#include <glge/common.h>

namespace glge::renderer::primitive::opengl
{
	class GLTexture
	{
	private:
		GLuint id;
		static GLuint load_texture(const TextureData & info)
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

		static GLuint load_texture_file(const TextureFileInfo & info)
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

	public:
		void activate() const
		{
			glBindTexture(GL_TEXTURE_2D, id);
		}

		GLuint getId() { return id; }

		GLTexture(const TextureData & info) : id(load_texture(info))
		{
		}

		GLTexture(const TextureFileInfo & info) : id(load_texture_file(info))
		{
		}

		GLTexture(const GLTexture &) = delete;
		GLTexture(GLTexture &&) = delete;

		GLTexture & operator=(const GLTexture &) = delete;
		GLTexture & operator=(GLTexture &&) = delete;

		~GLTexture()
		{
			glDeleteTextures(1, &id);
		}
	};
}
