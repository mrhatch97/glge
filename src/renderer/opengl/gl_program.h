#pragma once

#include "gl_common.h"

#include <glge/common.h>
#include <glge/util/util.h>

namespace glge::renderer::opengl
{
	class GLShader
	{
	public:
		using id_type = GLuint;

	private:
		const id_type id;

		static id_type load_shader_source(czstring code, GLenum shader_type)
		{
			using namespace std::literals::string_literals;

			id_type id = glCreateShader(shader_type);

			// Compile Vertex Shader
			glShaderSource(id, 1, &code, NULL);
			glCompileShader(id);

			// Check Vertex Shader
			GLint success = GL_FALSE;
			GLint info_log_length;

			glGetShaderiv(id, GL_COMPILE_STATUS, &success);
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
			if (!success)
			{
				unique_ptr<char[]> error = std::make_unique<char[]>(
					static_cast<size_t>(info_log_length) + 1);
				glGetShaderInfoLog(id, info_log_length, NULL, error.get());
				throw std::runtime_error(EXC_MSG(
					"Failed to compile shader source: "s + error.get()));
			}

			return id;
		}

	public:
		GLShader(czstring code, GLenum shader_type) :
			id(load_shader_source(code, shader_type))
		{}

		GLShader(const GLShader &) = delete;
		GLShader(GLShader &&) = delete;

		GLShader & operator=(const GLShader &) = delete;
		GLShader & operator=(GLShader &&) = delete;

		id_type get_id() const { return id; }

		~GLShader() { glDeleteShader(id); }
	};

	class GLProgram
	{
	private:
		using id_type = GLuint;

		const id_type id;

	public:
		GLProgram(const GLShader & vertex_shader,
				  const GLShader & fragment_shader) :
			id(glCreateProgram())
		{
			using namespace std::literals::string_literals;

			auto vertex_id = vertex_shader.get_id();
			auto fragment_id = fragment_shader.get_id();

			util::UniqueHandle attach_bind(
				[=] {
					glAttachShader(id, vertex_id);
					glAttachShader(id, fragment_id);
				},
				[=] {
					glDetachShader(id, vertex_id);
					glDetachShader(id, fragment_id);
				});

			glLinkProgram(id);

			// Check the program
			GLint success = GL_FALSE;
			GLint info_log_length;

			glGetProgramiv(id, GL_LINK_STATUS, &success);
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
			if (!success)
			{
				unique_ptr<char[]> error = std::make_unique<char[]>(
					static_cast<size_t>(info_log_length) + 1);
				glGetShaderInfoLog(id, info_log_length, NULL, error.get());
				throw std::runtime_error(
					EXC_MSG("Failed to link program: "s + error.get()));
			}
		}

		GLProgram(const GLProgram &) = delete;
		GLProgram(GLProgram &&) = delete;

		GLProgram & operator=(const GLProgram &) = delete;
		GLProgram & operator=(GLProgram &&) = delete;

		util::UniqueHandle activate() const
		{
			return util::UniqueHandle([&] { glUseProgram(id); },
									  [&] { glUseProgram(GL_NO_PROGRAM); });
		}

		GLint get_uniform(czstring name) const
		{
			return glGetUniformLocation(id, name);
		}

		GLint get_uniform(string name) const
		{
			return get_uniform(name.c_str());
		}

		~GLProgram() { glDeleteProgram(id); }
	};

	GLProgram load_simple_shader(czstring vertex_code, czstring fragment_code)
	{
		GLShader vertex_shader(vertex_code, GL_VERTEX_SHADER);
		GLShader fragment_shader(fragment_code, GL_FRAGMENT_SHADER);

		return GLProgram(vertex_shader, fragment_shader);
	}
}   // namespace glge::renderer::opengl