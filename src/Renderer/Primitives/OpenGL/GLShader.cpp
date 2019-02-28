#include "GLShader.h"

#include <glge/Util/Util.h>

namespace eng::render::primitive::opengl
{
	GLNormalShader::GLNormalShader() :
		uMVP(glGetUniformLocation(prog_id, "MVP"))
	{ 
	}

	void GLNormalShader::parameterize(const render::RenderParameters & render)
	{
		glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);

		DBG_OGL_ERROR_CHK(
			throw std::runtime_error(EXC_MSG("OpenGL error setting up shader"));
		);
	}

	GLColorShader::GLColorShader() :
		uMVP(glGetUniformLocation(prog_id, "MVP")),
		uColor(glGetUniformLocation(prog_id, "in_color"))
	{ }

	void GLColorShader::parameterize(const render::RenderParameters & render, vec3 color)
	{
		glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);
		glUniform3fv(uColor, 1, &color[0]);

		DBG_OGL_ERROR_CHK(
			throw std::runtime_error(EXC_MSG("OpenGL error setting up shader"));
		);
	}

	GLTextureShader::GLTextureShader() :
		uMVP(glGetUniformLocation(prog_id, "MVP")),
		uModel(glGetUniformLocation(prog_id, "model"))
	{ }

	void GLTextureShader::parameterize(const render::RenderParameters & render, const Texture & texture)
	{
		glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);
		glUniformMatrix4fv(uModel, 1, GL_FALSE, &render.M[0][0]);

		DBG_OGL_ERROR_CHK(
			throw std::runtime_error(EXC_MSG("OpenGL error setting up shader"));
		);
	}

	GLSkyboxShader::GLSkyboxShader() :
		uMVP(glGetUniformLocation(prog_id, "MVP"))
	{ }

	void GLSkyboxShader::parameterize(const render::RenderParameters & render, const Cubemap & skybox)
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

	GLEnvMapShader::GLEnvMapShader() :
		uMVP(glGetUniformLocation(prog_id, "MVP")),
		uModel(glGetUniformLocation(prog_id, "model")),
		uCam(glGetUniformLocation(prog_id, "camPos"))
	{ }

	void GLEnvMapShader::parameterize(const render::RenderParameters & render, const Cubemap & skybox)
	{
		glUniformMatrix4fv(uMVP, 1, GL_FALSE, &render.MVP[0][0]);
		glUniformMatrix4fv(uModel, 1, GL_FALSE, &render.M[0][0]);
		glUniform3f(uCam, render.settings.cam_pos.x, render.settings.cam_pos.y, render.settings.cam_pos.z);

		DBG_OGL_ERROR_CHK(
			throw std::runtime_error(EXC_MSG("OpenGL error setting up shader"));
		);
	}
}