/// <summary>Data objects for in-memory resources.</summary>
///
/// Contains data objects for representing engine resources in-memory.
///
/// \file primitive_data.h

#pragma once

#include <glge/common.h>
#include <glge/model_parser/types.h>

namespace glge::renderer::primitive
{
	using model_parser::Vertices;
	using model_parser::Normals;
	using model_parser::TexCoords;
	using model_parser::Indices;
	using model_parser::VertexData;
	using model_parser::NormalData;
	using model_parser::UVData;
	using model_parser::ModelData;
	using model_parser::ModelFileInfo;
	using model_parser::ModelFiletype;

	/// <summary>
	/// Data for a 3D model converted to an EBO-friendly format.
	/// </summary>
	/// A driver that supports only a single EBO per VAO requires
	/// model data be rearranged such that it can be rendered
	/// with only a single index list. This struct represents
	/// data that has been converted to this format.
	struct EBOModelData
	{
	private:
		EBOModelData() = default;

	public:
		/// <summary>Vertex list.</summary>
		Vertices vertices;
		/// <summary>Normal vector list.</summary>
		Normals normals;
		/// <summary>Uv list.</summary>
		TexCoords uvs;
		/// <summary>Index list. Indexes into all collections.</summary>
		Indices indices;

		/// <summary>
		/// Copy a set of EBOModelData.
		/// </summary>
		/// <param name="other">EBOModelData to copy from.</param>
		EBOModelData(const EBOModelData & other) = default;

		/// <summary>
		/// Move a set of EBOModelData.
		/// </summary>
		/// <param name="other">EBOModelData to move from.</param>
		EBOModelData(EBOModelData && other) = default;

		/// <summary>Convert a ModelData to an EBOModelData.</summary>
		/// <param name="data">
		/// ModelData to be converted. Data is copied.
		/// </param>
		EBOModelData(const ModelData & data);

		/// <summary>Convert a ModelData to an EBOModelData.</summary>
		/// <param name="data">ModelData to be converted. Data is moved.</param>
		/// <returns>Produced EBOModelData.</summary>
		EBOModelData(ModelData && data);
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
