#pragma once

#include "GLCommon.h"
#include <glge/Renderer/Resources/ResourceData.h>
#include <glge/Common.h>

namespace eng::render::primitive::opengl
{
	class GLModel
	{
	private:
		const GLsizei index_count;
		GLuint VAO[1], VBO[3], EBO[1];

	public:

		GLModel(const GLModel &) = delete;
		GLModel(GLModel &&) = delete;
		GLModel(const res::ModelData & info);
		~GLModel();

		GLModel & operator=(GLModel&&) = delete;
		GLModel & operator=(const GLModel &) = delete;

		void draw() const;

		const GLuint getVAO() const { return VAO[0]; }
	};
}