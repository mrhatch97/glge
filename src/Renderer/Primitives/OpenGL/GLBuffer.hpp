#pragma once

#include "GLCommon.h"

#include <glge/Common.h>
#include <glge/Util/Util.h>

namespace eng::render::primitive::opengl
{
	constexpr GLuint VERTEX_IDX = 0;
	constexpr GLuint NORMAL_IDX = 1;
	constexpr GLuint TEXCOR_IDX = 2;

	template<typename T>
	void bindAttribData(const GLuint vbo, const GLuint index, const std::vector<T> & data,
		const bool normalize)
	{
		using data_type = typename std::remove_reference<decltype(data)>::type::value_type;

		{
			util::UniqueHandle vboBind([vbo] { glBindBuffer(GL_ARRAY_BUFFER, vbo); }, [] { glBindBuffer(GL_ARRAY_BUFFER, 0); });

			glBufferData(GL_ARRAY_BUFFER, sizeof(data_type) * data.size(),
				data.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				static_cast<GLint>(data_type::length()),
				GL_FLOAT,
				normalize,
				static_cast<GLsizei>(sizeof(data_type)),
				static_cast<GLvoid *>(0)
			);
		}

		OGL_ERROR_CHK(
			throw std::runtime_error(EXC_MSG("Failed to load model attribute"));
		);
	}

	template<typename T>
	void bindElementArray(const GLuint ebo, const std::vector<T> & elements)
	{
		using data_type = typename std::remove_reference<decltype(elements)>::type::value_type;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data_type) * elements.size(),
			elements.data(), GL_STATIC_DRAW);

		OGL_ERROR_CHK(
			throw std::runtime_error(EXC_MSG("Failed to load model indices"));
		);
	}
}