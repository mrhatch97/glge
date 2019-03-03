#pragma once

#include "GLCommon.h"
#include <glge/Renderer/Resources/ResourceData.h>
#include <glge/Common.h>

#include <array>

namespace eng::render::primitive::opengl
{
	class GLCubemap
	{
	private:
		GLuint id;
		static GLuint loadCubemap(const res::CubemapData & info);

	public:
		GLCubemap(const res::CubemapFileInfo & info);
		GLCubemap(const res::CubemapData & info);
		GLCubemap(const GLCubemap &) = delete;
		GLCubemap(GLCubemap &&) = delete;

		GLCubemap & operator=(const GLCubemap &) = delete;
		GLCubemap & operator=(GLCubemap &&) = delete;

		void activate() const;

		~GLCubemap();
	};
}
