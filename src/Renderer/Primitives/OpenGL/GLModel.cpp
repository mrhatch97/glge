#include "GLModel.h"

#include "GLBuffer.hpp"

#include <glge/Util/Util.h>

namespace eng::render::primitive::opengl
{
	GLModel::GLModel(const res::ModelData & info) :
		index_count(static_cast<GLsizei>(info.indices.size()))
	{
		glGenVertexArrays(static_cast<GLsizei>(std::extent<decltype(VAO)>::value), VAO);
		glGenBuffers(static_cast<GLsizei>(std::extent<decltype(VBO)>::value), VBO);
		glGenBuffers(static_cast<GLsizei>(std::extent<decltype(EBO)>::value), EBO);

		if (GLenum err; (err = glGetError()) != GL_NO_ERROR)
		{
			throw std::runtime_error(EXC_MSG("Failed to generate requisite storage for model"));
		}

		{
			util::UniqueHandle vaoBind([&] { glBindVertexArray(VAO[0]); }, [] { glBindVertexArray(0); });

			bindAttribData(VBO[VERTEX_IDX], VERTEX_IDX, info.vertices, false);
			if (!info.normals.empty())
			{
				bindAttribData(VBO[NORMAL_IDX], NORMAL_IDX, info.normals, true);
			}
			if (!info.uvs.empty())
			{
				bindAttribData(VBO[TEXCOR_IDX], TEXCOR_IDX, info.uvs, false);
			}

			bindElementArray(EBO[0], info.indices);
		}
	}

	void GLModel::draw() const
	{
		util::UniqueHandle vaoBind(
			[&] { glBindVertexArray(VAO[0]); },
			[] { glBindVertexArray(0); }
		);

		DBG_OGL_ERROR_CHK(
			throw std::runtime_error(EXC_MSG("Error prior to render"));
		);

		glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);

		DBG_OGL_ERROR_CHK(
			throw std::runtime_error(EXC_MSG("Error after to render"));
		);
	}

	GLModel::~GLModel()
	{
		glDeleteVertexArrays(static_cast<GLsizei>(std::extent<decltype(VAO)>::value), VAO);
		glDeleteBuffers(static_cast<GLsizei>(std::extent<decltype(VBO)>::value), VBO);
		glDeleteBuffers(static_cast<GLsizei>(std::extent<decltype(EBO)>::value), EBO);
	}
}