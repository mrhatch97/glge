/// <summary>Data objects for in-memory resources.</summary>
///
/// Contains data objects for representing engine resources in-memory.
///
/// \file primitive_data.h

#pragma once

#include <glge/common.h>
#include <glge/obj_parser/obj_parser.h>

namespace glge::renderer::primitive
{
	using obj_parser::Vertices;
	using obj_parser::Normals;
	using obj_parser::TexCoords;
	using obj_parser::Indices;
	using obj_parser::VertexData;
	using obj_parser::NormalData;
	using obj_parser::UVData;

	/// <summary>
	/// File information data object for model files.
	/// </summary>
	struct ModelFileInfo
	{
		/// <summary>Path to model file.</summary>
		string filename;
	};

	struct EBOModelData;

	/// <summary>
	/// Data for a 3D model in program memory.
	/// </summary>
	struct ModelData
	{
		/// <summary>Vertices and vertex indices for the model.</summary>
		VertexData vertex_data;
		/// <summary>Normals and normal indices for the model.</summary>
		NormalData normal_data;
		/// <summary>Uvs and uv indices for the model.</summary>
		UVData uv_data;

		/// <summary>Load a set of ModelData from a file on disk.</summary>
		/// <param name="file_info">Descriptor for the model file.</param>
		/// <returns>Loaded ModelData.</returns>
		static ModelData from_file(const ModelFileInfo & file_info);

		/// <summary>Convert a ModelData to an EBOModelData.</summary>
		/// <param name="data">
		/// ModelData to be converted. Data is copied.
		/// </param>
		/// <returns>Produced EBOModelData.</summary>
		static EBOModelData to_EBO_data(const ModelData & data);

		/// <summary>Convert a ModelData to an EBOModelData.</summary>
		/// <param name="data">ModelData to be converted. Data is moved.</param>
		/// <returns>Produced EBOModelData.</summary>
		static EBOModelData to_EBO_data(ModelData && data);
	};

	/// <summary>
	/// Data for a 3D model converted to an EBO-friendly format.
	/// </summary>
	/// A driver that supports only a single EBO per VAO requires
	/// model data be rearranged such that it can be rendered
	/// with only a single index list. This struct represents
	/// data that has been converted to this format.
	struct EBOModelData
	{
		/// <summary>Vertex list.</summary>
		Vertices vertices;
		/// <summary>Normal vector list.</summary>
		Normals normals;
		/// <summary>Uv list.</summary>
		TexCoords uvs;
		/// <summary>Index list. Indexes into all collections.</summary>
		Indices indices;
	};

	/// <summary>
	/// File information data object for texture files.
	/// </summary>
	struct TextureFileInfo
	{
		/// <summary>Path to the texture file.</summary>
		string path;
	};

	/// <summary>
	/// File information data object for cubemap files.
	/// </summary>
	struct CubemapFileInfo
	{
		TextureFileInfo
			top,      ///< <summary>The top (up) cubemap file info.</summary>
			bottom,   ///< <summary>The bottom (down) cubemap file
					  ///< info.</summary>
			left,     ///< <summary>The left cubemap file info.</summary>
			right,    ///< <summary>The right cubemap file info.</summary>
			front,    ///< <summary>The front cubemap file info.</summary>
			back;     ///< <summary>The back cubemap file info.</summary>
	};
}   // namespace glge::renderer::primitive
