#pragma once

#include <glge/common.h>

#if GLGE_APPLE
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

namespace glge::renderer::opengl
{
	inline void throw_if_gl_error(string msg)
	{
		if (GLenum err; (err = glGetError()) != GL_NO_ERROR)
		{
			throw std::runtime_error(msg);
		}
	}

	constexpr GLuint GL_NO_PROGRAM = 0;
}   // namespace glge::renderer::opengl
