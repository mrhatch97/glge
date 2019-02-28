#pragma once

#include <glge/Common.h>

namespace eng::render::primitive
{
	class Model;
	class Texture;
	class Cubemap;
}

namespace eng::render::res
{
	struct ModelData
	{
		Vector<vec3> vertices;
		Vector<vec3> normals;
		Vector<vec2> uvs;
		Vector<unsigned int> indices;

		using manifest_type = primitive::Model;

		bool is_textured() const
		{
			return !uvs.empty();
		}
	};

	struct ModelFileInfo
	{
		String filename;
		bool is_textured;

		using data_type = ModelData;
		using manifest_type = primitive::Model;
	};

	struct TextureData
	{
		int width, height, channels;
		uptr<unsigned char[]> data;

		using manifest_type = primitive::Texture;
	};

	struct TextureFileInfo
	{
		String path;

		using data_type = TextureData;
		using manifest_type = primitive::Texture;
	};

	struct CubemapData
	{
		TextureData top, bottom, left, right, front, back;

		using manifest_type = primitive::Cubemap;
	};

	struct CubemapFileInfo
	{
		TextureFileInfo top, bottom, left, right, front, back;

		using data_type = CubemapData;
		using manifest_type = primitive::Cubemap;
	};
}