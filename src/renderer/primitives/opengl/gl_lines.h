#pragma once

#include "gl_common.h"
#include "gl_buffer.h"

#include <glge/common.h>
#include <glge/util/util.h>

namespace glge::renderer::primitive::opengl
{
	class GLLines
	{
	private:
		const GLsizei vertexCount;
		GLuint VAO[1], VBO[1];

	public:
		GLLines(const Vector<vec3> & points) : vertexCount(util::safe_cast<size_t, GLsizei>(points.size()))
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

		GLLines(const GLLines &) = delete;
		GLLines(GLLines &&) = delete;

		~GLLines()
		{
			glDeleteVertexArrays(static_cast<GLsizei>(std::extent<decltype(VAO)>::value), VAO);
			glDeleteBuffers(static_cast<GLsizei>(std::extent<decltype(VBO)>::value), VBO);
		}


		GLLines & operator=(const GLLines &) = delete;
		GLLines & operator=(GLLines &&) = delete;

		void render() const
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
	};
}
