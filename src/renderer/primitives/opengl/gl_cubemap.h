#pragma once

#include <SOIL.h>

#include "gl_common.h"
#include <glge/renderer/primitives/primitive_data.h>
#include <glge/common.h>

#include <array>

namespace glge::renderer::primitive::opengl
{
	class GLCubemap
	{
	private:
		GLuint id;
		static GLuint loadCubemap(const CubemapData & info)
		{
			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

			std::array<const TextureData *, 6> sides = { &info.right, &info.left, &info.top, &info.bottom, &info.front, &info.back };

			for (int i = 0; i < sides.size(); i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, sides[i]->width, sides[i]->height, 0, GL_RGB, GL_UNSIGNED_BYTE, sides[i]->data.get()
				);
			}

			// TODO https://www.khronos.org/opengl/wiki/Common_Mistakes#Creating_a_complete_texture
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			OGL_ERROR_CHK(
				throw std::runtime_error("Failed to create cubemap");
			);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			return textureID;
		}

	public:
		GLCubemap(const CubemapFileInfo & info) : id(
			SOIL_load_OGL_cubemap(
				info.right.path.c_str(), info.left.path.c_str(), info.top.path.c_str(),
				info.bottom.path.c_str(), info.back.path.c_str(), info.front.path.c_str(),
				SOIL_LOAD_RGB,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS
			))
		{
		}


		GLCubemap(const CubemapData & info) : id(loadCubemap(info))
		{
		}

		GLCubemap(const GLCubemap &) = delete;

		GLCubemap(GLCubemap &&) = delete;

		GLCubemap & operator=(const GLCubemap &) = delete;

		GLCubemap & operator=(GLCubemap &&) = delete;

		void activate() const
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		}

		~GLCubemap()
		{
			glDeleteTextures(1, &id);
		}
	};
}
