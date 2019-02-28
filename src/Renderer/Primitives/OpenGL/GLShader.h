#pragma once

#include "GLCommon.h"
#include <glge/Renderer/Primitives/Texture.h>
#include <glge/Renderer/Primitives/Cubemap.h>
#include <glge/Renderer/RenderSettings.h>
#include <glge/Util/Util.h>
#include <glge/Common.h>

#include <typeindex>

namespace eng::render::primitive::opengl
{
	using prog_id_t = GLuint;

	template<typename ShaderT>
	class GLShader
	{
	private:
		static prog_id_t load_shader(czstring vertex_code, czstring fragment_code)
		{
			using namespace std::literals::string_literals;

			// Create the shaders
			GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
			GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

			GLint success = GL_FALSE;
			GLint info_log_length;

			// Compile Vertex Shader
			glShaderSource(vertex_id, 1, &vertex_code, NULL);
			glCompileShader(vertex_id);

			// Check Vertex Shader
			glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);
			glGetShaderiv(vertex_id, GL_INFO_LOG_LENGTH, &info_log_length);
			if (!success)
			{
				uptr<char[]> error = std::make_unique<char[]>(static_cast<size_t>(info_log_length) + 1);
				glGetShaderInfoLog(vertex_id, info_log_length, NULL, error.get());
				throw std::runtime_error(EXC_MSG("Failed to compile vertex shader file: "s +
					error.get()));
			}

			// Compile Fragment Shader
			glShaderSource(fragment_id, 1, &fragment_code, NULL);
			glCompileShader(fragment_id);

			// Check Fragment Shader
			glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);
			glGetShaderiv(fragment_id, GL_INFO_LOG_LENGTH, &info_log_length);
			if (!success)
			{
				uptr<char[]> error = std::make_unique<char[]>(static_cast<size_t>(info_log_length) + 1);
				glGetShaderInfoLog(fragment_id, info_log_length, NULL, error.get());
				throw std::runtime_error(EXC_MSG("Failed to compile fragment shader file: "s +
					error.get()));
			}

			// Link the program
			GLuint program_id = glCreateProgram();
			glAttachShader(program_id, vertex_id);
			glAttachShader(program_id, fragment_id);
			glLinkProgram(program_id);

			// Check the program
			glGetProgramiv(program_id, GL_LINK_STATUS, &success);
			glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
			if (!success)
			{
				uptr<char[]> error = std::make_unique<char[]>(static_cast<size_t>(info_log_length) + 1);
				glGetShaderInfoLog(program_id, info_log_length, NULL, error.get());
				throw std::runtime_error(EXC_MSG("Failed to link program: "s +
					error.get()));
			}

			glDetachShader(program_id, vertex_id);
			glDetachShader(program_id, fragment_id);

			glDeleteShader(vertex_id);
			glDeleteShader(fragment_id);

			return program_id;
		}

		ShaderT & crtp_base()
		{
			return static_cast<ShaderT &>(*this);
		}

	protected:
		const prog_id_t prog_id;
	public:	
		GLShader() : prog_id(load_shader(ShaderT::vertex_code, ShaderT::fragment_code))
		{
		}

		util::UniqueHandle bind()
		{
			return util::UniqueHandle(
				[&] { glUseProgram(this->prog_id); },
				[&] { glUseProgram(GL_NO_PROGRAM); }
			);
		}

		~GLShader()
		{
			if (prog_id <= 0)
			{
				return;
			}

			glDeleteProgram(prog_id);
		}
	};

	class GLNormalShader : public GLShader<GLNormalShader>
	{
	private:
		const GLuint uMVP;
	public:
		static constexpr czstring vertex_code =
#include "generated/glsl/normal.vert.glsl"
			;

		static constexpr czstring fragment_code =
#include "generated/glsl/normal.frag.glsl"
			;

		GLNormalShader();

		void parameterize(const render::RenderParameters & render);
	};

	class GLColorShader : public GLShader<GLColorShader>
	{
	private:
		const GLuint uMVP, uColor;
	public:
		static constexpr czstring vertex_code =
#include "generated/glsl/color.vert.glsl"
			;

		static constexpr czstring fragment_code =
#include "generated/glsl/color.frag.glsl"
			;

		GLColorShader();

		void parameterize(const render::RenderParameters & render, vec3 color);
	};

	class GLTextureShader : public GLShader<GLTextureShader>
	{
	private:
		const GLuint uMVP, uModel;
	public:
		static constexpr czstring vertex_code =
#include "generated/glsl/tex.vert.glsl"
			;

		static constexpr czstring fragment_code =
#include "generated/glsl/tex.frag.glsl"
			;

		GLTextureShader();

		void parameterize(const render::RenderParameters & render, const Texture & texture);
	};

	class GLSkyboxShader : public GLShader<GLSkyboxShader>
	{
	private:
		const GLuint uMVP;
	public:
		static constexpr czstring vertex_code =
#include "generated/glsl/skybox.vert.glsl"
			;

		static constexpr czstring fragment_code =
#include "generated/glsl/skybox.frag.glsl"
			;

		GLSkyboxShader();

		void parameterize(const render::RenderParameters & render, const Cubemap & skybox);
	};

	class GLEnvMapShader : public GLShader<GLEnvMapShader>
	{
	private:
		const GLuint uMVP, uModel, uCam;
	public:
		static constexpr czstring vertex_code =
#include "generated/glsl/envmap.vert.glsl"
			;

		static constexpr czstring fragment_code =
#include "generated/glsl/envmap.frag.glsl"
			;

		GLEnvMapShader();

		void parameterize(const render::RenderParameters & render, const Cubemap & skybox);
	};
}