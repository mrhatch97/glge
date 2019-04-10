/// <summary>
/// Support for loading, parameterizing, and activating shaders.
/// </summary>
///
/// \file shader_program.h

#pragma once

#include <glge/common.h>
#include <glge/renderer/render_settings.h>
#include <glge/util/util.h>

#include <glge/renderer/primitives/texture.h>
#include <glge/renderer/primitives/cubemap.h>

#include <unordered_map>
#include <typeindex>

namespace glge::renderer::primitive
{
  /// <summary>
  /// Abstract base for shaders; declares common operations.
  /// </summary>
	class ShaderBase
	{
	public:
		ShaderBase() = default;

    /// <summary>
    /// Activate the shader so that it is the current shader program.
    /// </summary>
    /// <returns>Handle managing the binding of the shader.</returns>
		virtual util::UniqueHandle bind() = 0;

		virtual ~ShaderBase() = default;
	};

	template<typename DataT>
	struct ShaderInstance;

  /// <summary>
  /// Representation of a shader program.
  /// </summary>
  /// Represents the runtime loading of a shader program;
  /// can be instanced to bind shader parameters to this
  /// shader for future use.
  /// <typeparam name="DataT">
  /// Type of the data object used to parameterize the
  /// shader.
  /// </typeparam>
	template<typename DataT>
	class Shader : public ShaderBase
	{
	public:
    /// <summary>
    /// Load the shader corresponding to the given data type.
    /// </summary>
    /// <returns>Pointer to the loaded shader.</returns>
		static unique_ptr<Shader<DataT>> load();

		Shader() = default;

    /// <summary>
    /// Configures the shader to run using the given parameters during
    /// future rendering operations. Data object is copied.
    /// </summary>
    /// <param name="render">Parameters from the Renderer.</param>
    /// <param name="data">Data object used to parameterize the shader.</param>
		virtual void parameterize(
        const RenderParameters & render, 
        const DataT & data) = 0;

    /// <summary>
    /// Creates a shader instance, binding this shader to a set of
    /// parameter data.
    /// </summary>
    /// <param name="data">Data object to bind to.</param>
    /// <returns>
    /// ShaderInstance containing a reference to the Shader and a copy
    /// of the data given.
    /// </returns>
		ShaderInstance<DataT> instance(const DataT & data)
		{
			return ShaderInstance<DataT>(*this, data);
		}

    /// <summary>
    /// Creates a shader instance, binding this shader to a set of
    /// parameter data.
    /// </summary>
    /// <param name="data">Data object to bind to.</param>
    /// <returns>
    /// ShaderInstance containing a reference to the Shader and 
    /// the moved data.
    /// </returns>
		ShaderInstance<DataT> instance(DataT && data)
		{
			return ShaderInstance<DataT>(*this, std::move(data));
		}

    /// <summary>
    /// Creates a shader instance, binding this shader to a set of
    /// parameter data.
    /// </summary>
    /// <param name="args">Arguments used to construct data object.</param>
    /// <returns>
    /// ShaderInstance containing a reference to the Shader and a copy
    /// of the data given.
    /// </returns>
		template<typename...Args>
		ShaderInstance<DataT> instance(Args && ...args)
		{
			return instance(DataT{ std::forward<Args>(args)... });
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

  /// <summary>
  /// Abstract base for shader instances, declaring common operations.
  /// </summary>
	struct ShaderInstanceBase
	{
    /// <summary>
    /// Shader this is an instance of.
    /// </summary>
		ShaderBase & shader;

    /// <summary>
    /// Constructs a new ShaderInstanceBase pointing to the given shader.
    /// </summary>
		ShaderInstanceBase(ShaderBase & shader);

    /// <summary>
    /// Parameterize the active shader.
    /// </summary>
    /// Note that this does not bind the associated shader; the caller
    /// must ensure the shader is bound before invoking this function.
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

  /// <summary>
  /// Parameters for a NormalShader.
  /// </summary>
	struct NormalShaderData
	{
	};

  /// <summary>
  /// Parameters for a ColorShader.
  /// </summary>
	struct ColorShaderData
	{
    /// <summary>
    /// Color to shade with.
    /// </summary>
		vec3 color;
	};


  /// <summary>
  /// Parameters for a TextureShader.
  /// </summary>
	struct TextureShaderData
	{
    /// <summary>
    /// Texture to sample from.
    /// </summary>
		Texture & texture;
	};

  /// <summary>
  /// Parameters for a SkyboxShader.
  /// </summary>
	struct SkyboxShaderData
	{
    /// <summary>
    /// Skybox to sample from.
    /// </summary>
		Cubemap & skybox;
	};

  /// <summary>
  /// Parameters for an EnvMapShader.
  /// </summary>
	struct EnvMapShaderData
	{
    /// <summary>
    /// Environment map/skybox to sample from.
    /// </summary>
		Cubemap & skybox;
	};

  /// <summary>
  /// A normal shader.
  /// </summary>
  /// Colors points according to the value of their normal vector.
	using NormalShader = Shader<NormalShaderData>;

  /// <summary>
  /// A basic RGB color shader.
  /// </summary>
  /// Uniformly shades points a single RGB color.
	using ColorShader = Shader<ColorShaderData>;

  /// <summary>
  /// A basic 2D texture shader.
  /// </summary>
  /// Shades points by directly sampling a 2D texture.
	using TextureShader = Shader<TextureShaderData>;

  /// <summary>
  /// A cubic skybox shader.
  /// </summary>
  /// Shades a model by directly sampling a cubemap. Intended for
  /// use in shading cubic skyboxes.
	using SkyboxShader = Shader<SkyboxShaderData>;

  /// <summary>
  /// An environment map shader.
  /// </summary>
  /// Shades an object by simulating the perfect reflection of ambient light
  /// from the environment, supplied by a cubemap.
	using EnvMapShader = Shader<EnvMapShaderData>;

  /// <summary>
  /// Instance of a NormalShader.
  /// </summary>
	using NormalShaderInstance = ShaderInstance<NormalShaderData>;

  /// <summary>
  /// Instance of a ColorShader.
  /// </summary>
	using ColorShaderInstance = ShaderInstance<ColorShaderData>;

  /// <summary>
  /// Instance of a TextureShader.
  /// </summary>
	using TextureShaderInstance = ShaderInstance<TextureShaderData>;

  /// <summary>
  /// Instance of a SkyboxShader.
  /// </summary>
	using SkyboxShaderInstance = ShaderInstance<SkyboxShaderData>;

  /// <summary>
  /// Instance of a EnvMapShader.
  /// </summary>
	using EnvMapShaderInstance = ShaderInstance<EnvMapShaderData>;
}
