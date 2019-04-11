#include "gl_program.h"
#include <glge/renderer/primitives/texture.h>
#include <glge/renderer/primitives/cubemap.h>
#include <glge/renderer/primitives/shader_program.h>
#include <glge/renderer/render_settings.h>
#include <glge/util/util.h>
#include <glge/common.h>

namespace glge::renderer::primitive
{
	namespace opengl
	{
		using namespace glge::renderer::opengl;

		template<typename DataT, typename ShaderT>
		class GLShader : public Shader<DataT>
		{
		protected:
			const GLProgram prog;
		public:
			GLShader() : 
        prog(renderer::opengl::load_simple_shader(ShaderT::vertex_code, 
              ShaderT::fragment_code))
			{
			}

			util::UniqueHandle bind() override
			{
				return prog.activate();
			}

			virtual ~GLShader() = default;
		};

		class GLNormalShader : public GLShader<NormalShaderData, GLNormalShader>
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
				uMVP(prog.get_uniform("MVP"))
			{
			}

			void parameterize(const RenderParameters & render, 
          const NormalShaderData &) override
			{
				glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);

				if constexpr (debug)
				{
					renderer::opengl::throw_if_gl_error(EXC_MSG("OpenGL error setting up shader"));
				}
			}
		};

		class GLColorShader : public GLShader<ColorShaderData, GLColorShader>
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
				uMVP(prog.get_uniform("MVP")),
				uColor(prog.get_uniform("in_color"))
			{ }

			void parameterize(const RenderParameters & render, const ColorShaderData & data) override
			{
				glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);
				glUniform3fv(uColor, 1, &data.color[0]);

				if constexpr (debug)
				{
					renderer::opengl::throw_if_gl_error(EXC_MSG("OpenGL error setting up shader"));
				}
			}
		};

		class GLTextureShader : public GLShader<TextureShaderData, GLTextureShader>
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
				uMVP(prog.get_uniform("MVP")),
				uModel(prog.get_uniform("model"))
			{ }

			void parameterize(const RenderParameters & render, const TextureShaderData & data) override
			{
				data.texture.activate();

				glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);
				glUniformMatrix4fv(uModel, 1, GL_FALSE, &render.M[0][0]);

				if constexpr (debug)
				{
					renderer::opengl::throw_if_gl_error(EXC_MSG("OpenGL error setting up shader"));
				}
			}
		};

		class GLSkyboxShader : public GLShader<SkyboxShaderData, GLSkyboxShader>
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
				uMVP(prog.get_uniform("MVP"))
			{ }

			util::UniqueHandle bind() override
			{
				return std::move(
					GLShader<SkyboxShaderData, GLSkyboxShader>::bind()
					.chain(
						[&] { glCullFace(GL_FRONT); },
						[&] { glCullFace(GL_BACK); }
					));
			}

			void parameterize(const RenderParameters & render, const SkyboxShaderData & data) override
			{
				data.skybox.activate();

				glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);

				if constexpr (debug)
				{
					renderer::opengl::throw_if_gl_error(EXC_MSG("OpenGL error setting up shader"));
				}
			}
		};

		class GLEnvMapShader : public GLShader<EnvMapShaderData, GLEnvMapShader>
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
				uMVP(prog.get_uniform("MVP")),
				uModel(prog.get_uniform("model")),
				uCam(prog.get_uniform("camPos"))
			{ }

			void parameterize(const RenderParameters & render, const EnvMapShaderData & data) override
			{
				data.skybox.activate();

				glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);
				glUniformMatrix4fv(uModel, 1, GL_FALSE, &render.M[0][0]);

				vec3 cam_pos = render.settings.camera->placement.get_position();
				glUniform3fv(uCam, cam_pos.length(), &cam_pos[0]);

				if constexpr (debug)
				{
					renderer::opengl::throw_if_gl_error(EXC_MSG("OpenGL error setting up shader"));
				}
			}
		};
	}

	template<>
	unique_ptr<NormalShader> NormalShader::load()
	{
		return std::make_unique<opengl::GLNormalShader>();
	}

	template<>
	unique_ptr<ColorShader> ColorShader::load()
	{
		return std::make_unique<opengl::GLColorShader>();
	}

	template<>
	unique_ptr<TextureShader> TextureShader::load()
	{
		return std::make_unique<opengl::GLTextureShader>();
	}

	template<>
	unique_ptr<SkyboxShader> SkyboxShader::load()
	{
		return std::make_unique<opengl::GLSkyboxShader>();
	}

	template<>
	unique_ptr<EnvMapShader> EnvMapShader::load()
	{
		return std::make_unique<opengl::GLEnvMapShader>();
	}
}
