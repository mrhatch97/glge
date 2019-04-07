#pragma once

#include <glge/common.h>
#include <glge/renderer/render_settings.h>
#include <glge/util/util.h>

#include <glge/renderer/primitives/texture.h>
#include <glge/renderer/primitives/cubemap.h>

#include <unordered_map>
#include <typeindex>
#include <variant>

namespace glge::renderer::primitive
{
	class ShaderBase
	{
	public:
		ShaderBase() = default;

		virtual util::UniqueHandle bind() = 0;

		virtual ~ShaderBase() = default;
	};

	template<typename DataT>
	struct ShaderInstance;

	template<typename DataT>
	class Shader : public ShaderBase
	{
	public:
		static unique_ptr<Shader<DataT>> load();

		Shader() = default;

		virtual void parameterize(const RenderParameters & render, const DataT & data) = 0;

		template<typename...Args>
		ShaderInstance<DataT> instance(Args && ...args)
		{
			return instance(DataT{ std::forward<Args>(args)... });
		}

		ShaderInstance<DataT> instance(const DataT & data)
		{
			return ShaderInstance<DataT>(*this, data);
		}

		ShaderInstance<DataT> instance(DataT && data)
		{
			return ShaderInstance<DataT>(*this, std::move(data));
		}

		virtual ~Shader() = default;
	};

	class ShaderManager
	{
	private:
		std::unordered_map<std::type_index, unique_ptr<ShaderBase>> shaders;

	public:
		ShaderManager();

		template<typename ShaderT>
		ShaderT & load()
		{
			unique_ptr<ShaderT> shader = std::make_unique<ShaderT>();
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

	struct ShaderInstanceBase
	{
		ShaderBase & shader;

		ShaderInstanceBase(ShaderBase & shader);

		virtual void operator()(const RenderParameters & render) const = 0;

		virtual ~ShaderInstanceBase() = default;
	};

	template<typename DataT>
	struct ShaderInstance : public ShaderInstanceBase
	{
		DataT data;

		ShaderInstance(Shader<DataT> & shader, const DataT & data)
			: ShaderInstanceBase(shader), data(data)
		{ }

		ShaderInstance(Shader<DataT> & shader, DataT && data)
			: ShaderInstanceBase(shader), data(std::move(data))
		{ }

		void operator()(const RenderParameters & render) const override
		{
			static_cast<Shader<DataT> &>(shader).parameterize(render, data);
		}
	};

	struct NormalShaderData
	{
	};

	struct ColorShaderData
	{
		vec3 color;
	};

	struct TextureShaderData
	{
		Texture & texture;
	};

	struct SkyboxShaderData
	{
		Cubemap & skybox;
	};

	struct EnvMapShaderData
	{
		Cubemap & skybox;
	};

	using NormalShader = Shader<NormalShaderData>;
	using ColorShader = Shader<ColorShaderData>;
	using TextureShader = Shader<TextureShaderData>;
	using SkyboxShader = Shader<SkyboxShaderData>;
	using EnvMapShader = Shader<EnvMapShaderData>;

	using NormalShaderInstance = ShaderInstance<NormalShaderData>;
	using ColorShaderInstance = ShaderInstance<ColorShaderData>;
	using TextureShaderInstance = ShaderInstance<TextureShaderData>;
	using SkyboxShaderInstance = ShaderInstance<SkyboxShaderData>;
	using EnvMapShaderInstance = ShaderInstance<EnvMapShaderData>;
}
