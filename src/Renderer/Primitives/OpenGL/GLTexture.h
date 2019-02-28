#pragma once

#include "GLCommon.h"
#include <glge/Renderer/Resources/ResourceData.h>

#include <glge/Common.h>

namespace eng::render::primitive::opengl
{
	class GLTexture
	{
	private:
		GLuint id;
		static GLuint load_texture(const res::TextureData & info);
		static GLuint load_texture_file(const res::TextureFileInfo & info);
	public:
		void activate() const;
		GLuint getId() { return id; }
		GLTexture(const res::TextureData & info);
		GLTexture(const res::TextureFileInfo & info);
		GLTexture(const GLTexture &) = delete;
		GLTexture(GLTexture &&) = delete;

		GLTexture & operator=(const GLTexture &) = delete;
		GLTexture & operator=(GLTexture &&) = delete;

		~GLTexture();
	};
}
