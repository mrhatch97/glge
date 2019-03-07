#pragma once

namespace glge::renderer::primitive
{
	struct ModelFileInfo
	{
		String filename;
		bool is_textured;
	};

	struct ModelData
	{
		Vector<vec3> vertices;
		Vector<vec3> normals;
		Vector<vec2> uvs;
		Vector<unsigned int> indices;

		bool is_textured() const
		{
			return !uvs.empty();
		}
	};

	struct TextureFileInfo
	{
		String path;
	};

	struct TextureData
	{
		int width, height, channels;
		uptr<unsigned char[]> data;
	};

	struct CubemapFileInfo
	{
		TextureFileInfo top, bottom, left, right, front, back;
	};

	struct CubemapData
	{
		TextureData top, bottom, left, right, front, back;
	};
}
