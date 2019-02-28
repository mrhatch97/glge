#pragma once

#include <glge/Common.h>
#include <glge/Renderer/RenderSettings.h>
#include <glge/Util/Util.h>

#include <glge/Renderer/Primitives/ShaderCompat.h>
#include <glge/Renderer/Primitives/Texture.h>
#include <glge/Renderer/Primitives/Cubemap.h>

#include <unordered_map>
#include <typeindex>
#include <variant>

namespace eng::render::primitive
{
	class ShaderBase
	{
	public:
		ShaderBase();

		virtual ~ShaderBase();
	};

	template<typename TagT>
	class Shader : public ShaderBase
	{
	private:
		uptr<typename TagT::Implementation> shader;
	public:
		Shader();

		util::UniqueHandle bind();

		template<typename...Args>
		void parameterize(const render::RenderParameters & render, Args&&... args)
		{
			shader->parameterize(render, std::forward<Args>(args)...);
		}

		template<typename...Args>
		typename TagT::Instance instance(Args&&... args)
		{
			return typename TagT::Instance(*this, std::forward<Args>(args)...);
		}

		~Shader();
	};

	using NormalShader = Shader<NormalShaderTag>;
	using ColorShader = Shader<ColorShaderTag>;
	using TextureShader = Shader<TextureShaderTag>;
	using SkyboxShader = Shader<SkyboxShaderTag>;
	using EnvMapShader = Shader<EnvMapShaderTag>;

	class ShaderManager
	{
	private:
		std::unordered_map<std::type_index, uptr<ShaderBase>> shaders;

	public:
		ShaderManager();

		template<typename ShaderT>
		ShaderT & load()
		{
			uptr<ShaderT> shader = std::make_unique<ShaderT>();
			ShaderT & shader_ref = *shader;

			shaders.emplace(typeid(ShaderT), std::move(shader));

			return shader_ref;
		}
		
		template<typename ShaderT>
		ShaderT & get()
		{
			return shaders.at(typeid(ShaderT));
		}

		~ShaderManager();
	};

	class ShaderInstance
	{
	public:
		virtual util::UniqueHandle operator()(const render::RenderParameters & render) const = 0;

		virtual ~ShaderInstance();
	};

	class NormalShaderInstance : public ShaderInstance
	{
		NormalShader & shader;
	public:
		NormalShaderInstance(NormalShader & shader);

		util::UniqueHandle operator()(const render::RenderParameters & render) const override;
	};

	class ColorShaderInstance : public ShaderInstance
	{
		ColorShader & shader;
		const vec3 color;
	public:
		ColorShaderInstance(ColorShader & shader, vec3 color);

		util::UniqueHandle operator()(const render::RenderParameters & render) const override;
	};

	class TextureShaderInstance : public ShaderInstance
	{
		TextureShader & shader;
		Texture & texture;
	public:
		TextureShaderInstance(TextureShader & shader, Texture & texture);

		util::UniqueHandle operator()(const render::RenderParameters & render) const override;
	};

	class SkyboxShaderInstance : public ShaderInstance
	{
		SkyboxShader & shader;
		Cubemap & skybox;
	public:
		SkyboxShaderInstance(SkyboxShader & shader, Cubemap & skybox);

		util::UniqueHandle operator()(const render::RenderParameters & render) const override;
	};

	class EnvMapShaderInstance : public ShaderInstance
	{
		EnvMapShader & shader;
		Cubemap & skybox;
	public:
		EnvMapShaderInstance(EnvMapShader & shader, Cubemap & skybox);

		util::UniqueHandle operator()(const render::RenderParameters & render) const override;
	};
}