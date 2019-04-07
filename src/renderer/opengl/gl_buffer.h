#pragma once

#include "gl_common.h"

#include <glge/common.h>
#include <glge/util/util.h>

namespace glge::renderer::primitive::opengl
{
	constexpr GLuint vertex_index = 0;
	constexpr GLuint normal_index = 1;
	constexpr GLuint texcor_index = 2;
	constexpr GLvoid * zero_offset = 0;

	template<typename T>
	void bind_attrib_data(const GLuint vbo, const GLuint index, const vector<T> & data,
		const bool normalize)
	{
		using data_type = typename std::remove_reference_t<decltype(data)>::value_type;

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
				zero_offset
			);
		}

		renderer::opengl::throw_if_gl_error(EXC_MSG("Failed to load model attribute"));
	}

	template<typename T>
	void bind_element_array(const GLuint ebo, const vector<T> & elements)
	{
		using data_type = typename std::remove_reference_t<decltype(elements)>::value_type;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data_type) * elements.size(),
			elements.data(), GL_STATIC_DRAW);

		renderer::opengl::throw_if_gl_error(EXC_MSG("Failed to load model indices"));
	}
}
