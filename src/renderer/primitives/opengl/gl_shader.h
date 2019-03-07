#pragma once

#include "gl_common.h"
#include <glge/renderer/primitives/texture.h>
#include <glge/renderer/primitives/cubemap.h>
#include <glge/renderer/render_settings.h>
#include <glge/util/util.h>
#include <glge/common.h>

#include <typeindex>

namespace glge::renderer::primitive::opengl
{
	using prog_id_t = GLuint;
	using namespace glge::renderer::opengl;

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

    GLNormalShader() :
      uMVP(glGetUniformLocation(prog_id, "MVP"))
    {
    }

    void parameterize(const RenderParameters & render)
    {
      glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);

      DBG_OGL_ERROR_CHK(
        throw std::runtime_error(EXC_MSG("OpenGL error setting up shader"));
      );
    }
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

    GLColorShader() :
      uMVP(glGetUniformLocation(prog_id, "MVP")),
      uColor(glGetUniformLocation(prog_id, "in_color"))
    { }

    void parameterize(const RenderParameters & render, vec3 color)
    {
      glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);
      glUniform3fv(uColor, 1, &color[0]);

      DBG_OGL_ERROR_CHK(
        throw std::runtime_error(EXC_MSG("OpenGL error setting up shader"))
			);
    }
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

    GLTextureShader() :
      uMVP(glGetUniformLocation(prog_id, "MVP")),
      uModel(glGetUniformLocation(prog_id, "model"))
    { }

    void parameterize(const RenderParameters & render, const Texture & texture)
    {
      glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);
      glUniformMatrix4fv(uModel, 1, GL_FALSE, &render.M[0][0]);

      DBG_OGL_ERROR_CHK(
        throw std::runtime_error(EXC_MSG("OpenGL error setting up shader"));
      );
    }
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

    GLSkyboxShader() :
      uMVP(glGetUniformLocation(prog_id, "MVP"))
    { }

    void parameterize(const RenderParameters & render, const Cubemap & skybox)
    {
      // TODO deal with binds/handles
      /*
      util::UniqueHandle shaderBind(
        [&] { glUseProgram(prog_id); skybox.activate(); glCullFace(GL_FRONT); },
        [&] { glCullFace(GL_BACK); glUseProgram(0); }
      );
      */

      glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);

      DBG_OGL_ERROR_CHK(
        throw std::runtime_error(EXC_MSG("OpenGL error setting up shader"));
      );
    }
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

    GLEnvMapShader() :
      uMVP(glGetUniformLocation(prog_id, "MVP")),
      uModel(glGetUniformLocation(prog_id, "model")),
      uCam(glGetUniformLocation(prog_id, "camPos"))
    { }

    void parameterize(const RenderParameters & render, const Cubemap & skybox)
    {
      glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);
      glUniformMatrix4fv(uModel, 1, GL_FALSE, &render.M[0][0]);
      glUniform3f(uCam, render.settings.cam_pos.x, render.settings.cam_pos.y, render.settings.cam_pos.z);

      DBG_OGL_ERROR_CHK(
        throw std::runtime_error(EXC_MSG("OpenGL error setting up shader"));
      );
    }
	};
}
