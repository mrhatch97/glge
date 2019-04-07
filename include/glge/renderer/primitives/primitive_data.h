#pragma once

#include <glge/common.h>

namespace glge::renderer::primitive
{
	struct ModelFileInfo
	{
		string filename;
		bool is_textured;
	};

	using VertexPoints = vector<vec3>;
	using NormalPoints = vector<vec3>;
	using UVPoints = vector<vec2>;
	using Indices = vector<unsigned int>;

	struct VertexData
	{
		VertexPoints points;
		Indices indices;
	};

	struct NormalData
	{
		NormalPoints points;
		Indices indices;
	};

	struct UVData
	{
		UVPoints points;
		Indices indices;
	};

	struct EBOModelData;

	struct ModelData
	{
		VertexData vertex_data;
		NormalData normal_data;
		UVData uv_data;

		static ModelData from_file(const ModelFileInfo & file_info);
		static EBOModelData to_EBO_data(const ModelData & data);
		static EBOModelData to_EBO_data(ModelData && data);
	};

	struct EBOModelData
	{
		VertexPoints vertices;
		NormalPoints normals;
		UVPoints uvs;
		Indices indices;
	};

	struct TextureFileInfo
	{
		string path;
	};

	struct CubemapFileInfo
	{
		TextureFileInfo top, bottom, left, right, front, back;
	};
}
