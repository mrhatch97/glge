#pragma once

#include "GLCommon.h"

#include <glge/Common.h>

namespace eng::render::primitive::opengl
{
	class GLLines
	{
	private:
		const GLsizei vertexCount;
		GLuint VAO[1], VBO[1];

	public:
		GLLines(const Vector<vec3> & points);
		GLLines(const GLLines &) = delete;
		GLLines(GLLines &&) = delete;
		~GLLines();

		GLLines & operator=(const GLLines &) = delete;
		GLLines & operator=(GLLines &&) = delete;

		void render() const;
	};
}