#pragma once

namespace eng::render::primitive
{
	/* OPENGL SHADER IMPLEMENTATION */
	namespace opengl
	{
		class GLNormalShader;
		class GLColorShader;
		class GLTextureShader;
		class GLSkyboxShader;
		class GLEnvMapShader;
	}

	namespace
	{
		using NormalShaderImpl = opengl::GLNormalShader;
		using ColorShaderImpl = opengl::GLColorShader;
		using TextureShaderImpl = opengl::GLTextureShader;
		using SkyboxShaderImpl = opengl::GLSkyboxShader;
		using EnvMapShaderImpl = opengl::GLEnvMapShader;
	}
	/********************************/

	class NormalShaderInstance;
	class ColorShaderInstance;
	class TextureShaderInstance;
	class SkyboxShaderInstance;
	class EnvMapShaderInstance;

	struct NormalShaderTag
	{
		using Implementation = NormalShaderImpl;
		using Instance = NormalShaderInstance;
	};

	struct ColorShaderTag
	{
		using Implementation = ColorShaderImpl;
		using Instance = ColorShaderInstance;
	};

	struct TextureShaderTag
	{
		using Implementation = TextureShaderImpl;
		using Instance = TextureShaderInstance;
	};

	struct SkyboxShaderTag
	{
		using Implementation = SkyboxShaderImpl;
		using Instance = SkyboxShaderInstance;
	};

	struct EnvMapShaderTag
	{
		using Implementation = EnvMapShaderImpl;
		using Instance = EnvMapShaderInstance;
	};
}