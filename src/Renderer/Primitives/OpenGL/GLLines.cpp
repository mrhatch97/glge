#include "GLLines.h"

#include "GLBuffer.hpp"

#include "GLCommon.h"
#include <glge/Util/Util.h>

#include <algorithm>

namespace eng::render::primitive::opengl
{
	GLLines::GLLines(const Vector<vec3> & points) : 
		vertexCount(static_cast<GLsizei>(points.size()))
	{
		glGenVertexArrays(static_cast<GLsizei>(std::extent<decltype(VAO)>::value), VAO);
		glGenBuffers(static_cast<GLsizei>(std::extent<decltype(VBO)>::value), VBO);
		
		if (GLenum err; (err = glGetError()) != GL_NO_ERROR)
		{
			throw std::runtime_error(EXC_MSG("Failed to generate requisite storage for lines"));
		}

		{
			util::UniqueHandle vaoBind([&] { glBindVertexArray(VAO[0]); }, [] { glBindVertexArray(0); });

			bindAttribData(VBO[VERTEX_IDX], VERTEX_IDX, points, false);
		}
	}

	void GLLines::render() const
	{
		util::UniqueHandle vaoBind(
			[&] { glBindVertexArray(VAO[0]); },
			[] { glBindVertexArray(0); }
		);

		DBG_OGL_ERROR_CHK(
			throw std::runtime_error(EXC_MSG("Error prior to render"));
		);

		glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

		DBG_OGL_ERROR_CHK(
			throw std::runtime_error(EXC_MSG("Error during render"));
		);
	}

	GLLines::~GLLines()
	{
		glDeleteVertexArrays(static_cast<GLsizei>(std::extent<decltype(VAO)>::value), VAO);
		glDeleteBuffers(static_cast<GLsizei>(std::extent<decltype(VBO)>::value), VBO);
	}
}