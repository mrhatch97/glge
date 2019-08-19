#pragma once

#include <glge/common.h>
#include <glge/util/util.h>

/// <summary>
/// Data representations for model files.
/// </summary>
namespace glge::model_parser
{
	/// <summary>
	/// Strong typedef of glm::vec3 representing a vertex in a 3D model.
	/// </summary>
	struct Vertex : public util::Newtype<Vertex, vec3>
	{
		/// <summary>
		/// Default constructor.
		/// </summary>
		Vertex() = default;

		/// <summary>
		/// Constructs a Vertex from the given vec3.
		/// </summary>
		/// <param name="value">vec3 to wrap.</param>
		explicit constexpr Vertex(vec3 value) noexcept :
			Newtype<Vertex, vec3>(value)
		{}

		/// <summary>
		/// Re-exposure of glm::vec3::length()
		/// </summary>
		/// <returns>Number of vector components.</returns>
		constexpr static size_t length()
		{
			// Why is this not constexpr? Ask GLM
			// return vec3::length();
			return 3;
		}
	};

	/// <summary>
	/// Strong typedef of glm::vec3 representing a normal in a 3D model.
	/// </summary>
	struct Normal : public util::Newtype<Normal, vec3>
	{
		/// <summary>
		/// Default constructor.
		/// </summary>
		Normal() = default;

		/// <summary>
		/// Constructs a Normal from the given vec3.
		/// </summary>
		/// <param name="value">vec3 to wrap.</param>
		explicit constexpr Normal(vec3 value) noexcept :
			Newtype<Normal, vec3>(value)
		{}

		/// <summary>
		/// Re-exposure of glm::vec3::length()
		/// </summary>
		/// <returns>Number of vector components.</returns>
		constexpr static size_t length()
		{
			// Why is this not constexpr? Ask GLM
			// return vec3::length();
			return 3;
		}
	};

	/// <summary>
	/// Strong typedef of glm::vec2 representing a texture coordinate in a 3D
	/// model.
	/// </summary>
	struct TexCoord : public util::Newtype<TexCoord, vec2>
	{
		/// <summary>
		/// Default constructor.
		/// </summary>
		TexCoord() = default;

		/// <summary>
		/// Constructs a Normal from the given vec2.
		/// </summary>
		/// <param name="value">vec2 to wrap.</param>
		explicit constexpr TexCoord(vec2 value) noexcept :
			Newtype<TexCoord, vec2>(value)
		{}

		/// <summary>
		/// Re-exposure of glm::vec2::length()
		/// </summary>
		/// <returns>Number of vector components.</returns>
		constexpr static size_t length()
		{
			// Why is this not constexpr? Ask GLM
			// return vec2::length();
			return 2;
		}
	};

	/// <summary>
	/// Strong typedef of std::size_t representing an index in a 3D model
	/// model.
	/// </summary>
	struct Index : public util::Newtype<Index, size_t>
	{
		/// <summary>
		/// Default constructor.
		/// </summary>
		Index() = default;

		/// <summary>
		/// Constructs an Index from the given std::size_t.
		/// </summary>
		/// <param name="value">size_t to wrap.</param>
		explicit constexpr Index(size_t value) noexcept :
			Newtype<Index, size_t>(value)
		{}
	};

	/// <summary>Type alias for collection of vertices.</summary>
	using Vertices = vector<Vertex>;
	/// <summary>Type alias for collection of normals.</summary>
	using Normals = vector<Normal>;
	/// <summary>Type alias for collection of uvs.</summary>
	using TexCoords = vector<TexCoord>;
	/// <summary>Type alias for collection of indices.</summary>
	using Indices = vector<Index>;

	/// <summary>
	/// Representation of a collection and a set of indices
	/// into that collection.
	/// </summary>
	/// <typeparam name="CollectionT">Type of the collection.</typeparam>
	template<typename CollectionT>
	struct Indexed
	{
		/// <summary>
		/// The collection to index into.
		/// </summary>
		CollectionT points;
		/// <summary>
		/// Indices into the collection.
		/// </summary>
		Indices indices;
	};

	/// <summary>
	/// Data object containing a set of vertex points and their
	/// mapped indices.
	/// </summary>
	using VertexData = Indexed<Vertices>;

	/// <summary>
	/// Data object containing a set of model normals and their
	/// mapped indices.
	/// </summary>
	using NormalData = Indexed<Normals>;

	/// <summary>
	/// Data object containing a set of model uvs and their
	/// mapped indices.
	/// </summary>
	using UVData = Indexed<TexCoords>;

	/// <summary>
	/// Supported file types for model file loading.
	/// </summary>
	enum class ModelFiletype
	{
		Auto,
		Object
	};

	/// <summary>
	/// Info for loading a model file from disk.
	/// </summary>
	struct ModelFileInfo
	{
		/// <summary>
		/// Path to model file.
		/// </summary>
		czstring filepath;

		/// <summary>
		/// Type of file to load or auto to determine
		/// from file extension.
		/// </summary>
		ModelFiletype filetype;
	};

	/// <summary>
	/// Representation of a model file.
	/// </summary>
	struct ModelData
	{
		/// <summary>Vertices and vertex indices for the model.</summary>
		VertexData vertex_data;
		/// <summary>Normals and normal indices for the model.</summary>
		NormalData normal_data;
		/// <summary>Uvs and uv indices for the model.</summary>
		UVData uv_data;

		/// <summary>
		/// Load a set of model data from the given file.
		/// </summary>
		/// <param name="file_info">
		/// Descriptor for the file to load from.
		/// </param>
		/// <returns>Loaded model data.</returns>
		static ModelData from_file(ModelFileInfo file_info);
	};
}   // namespace glge::model_parser
