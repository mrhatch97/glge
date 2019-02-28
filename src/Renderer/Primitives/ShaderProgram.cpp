#include "glge/Renderer/Primitives/ShaderProgram.h"

#include <glge/Common.h>

#include "GLShader.h"

namespace eng::render::primitive
{
	ShaderBase::ShaderBase() = default;

	ShaderBase::~ShaderBase() = default;

	template<typename TagT>
	Shader<TagT>::Shader() : shader(std::make_unique<typename TagT::Implementation>())
	{ 
	}

	template<typename TagT>
	util::UniqueHandle Shader<TagT>::bind()
	{
		return shader->bind();
	}

	template<typename TagT>
	Shader<TagT>::~Shader() = default;

	template class Shader<NormalShaderTag>;
	template class Shader<ColorShaderTag>;
	template class Shader<TextureShaderTag>;
	template class Shader<SkyboxShaderTag>;
	template class Shader<EnvMapShaderTag>;

	ShaderManager::ShaderManager() = default;

	ShaderManager::~ShaderManager() = default;

	ShaderInstance::~ShaderInstance() = default;

	NormalShaderInstance::NormalShaderInstance(NormalShader & shader) : shader(shader)
	{
	}

	util::UniqueHandle NormalShaderInstance::operator()(const render::RenderParameters & render) const
	{
		auto bind = shader.bind();
		shader.parameterize(render);
		return std::move(bind);
	}

	ColorShaderInstance::ColorShaderInstance(ColorShader & shader, vec3 color) : 
		shader(shader), color(color)
	{
	}

	util::UniqueHandle ColorShaderInstance::operator()(const render::RenderParameters & render) const
	{
		auto bind = shader.bind();
		shader.parameterize(render, color);
		return std::move(bind);
	}

	TextureShaderInstance::TextureShaderInstance(TextureShader & shader, Texture & texture) :
		shader(shader), texture(texture)
	{
	}

	util::UniqueHandle TextureShaderInstance::operator()(const render::RenderParameters & render) const
	{
		auto bind = shader.bind();
		shader.parameterize(render, texture);
		return std::move(bind);
	}

	SkyboxShaderInstance::SkyboxShaderInstance(SkyboxShader & shader, Cubemap & skybox) :
		shader(shader), skybox(skybox)
	{
	}

	util::UniqueHandle SkyboxShaderInstance::operator()(const render::RenderParameters & render) const
	{
		auto bind = shader.bind();
		shader.parameterize(render, skybox);
		return std::move(bind);
	}

	EnvMapShaderInstance::EnvMapShaderInstance(EnvMapShader & shader, Cubemap & skybox) :
		shader(shader), skybox(skybox)
	{
	}

	util::UniqueHandle EnvMapShaderInstance::operator()(const render::RenderParameters & render) const
	{
		auto bind = shader.bind();
		shader.parameterize(render, skybox);
		return std::move(bind);
	}
}