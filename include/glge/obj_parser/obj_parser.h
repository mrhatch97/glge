/// <summary>
/// Data representations for object files and object file
/// parsing functions.
/// </summary>
///
/// \file obj_parser.h

#pragma once

#include <glge/common.h>
#include <glge/util/util.h>

namespace glge::obj_parser
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

	/// <summary>Type alias for collection of vertices.</summary>
	using Vertices = vector<Vertex>;
	/// <summary>Type alias for collection of normals.</summary>
	using Normals = vector<Normal>;
	/// <summary>Type alias for collection of uvs.</summary>
	using TexCoords = vector<TexCoord>;
	/// <summary>Type alias for collection of indices.</summary>
	using Indices = vector<unsigned int>;

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
	/// Representation of an object file.
	/// </summary>
	struct Object
	{
		/// <summary>Vertices and vertex indices for the model.</summary>
		VertexData vertex_data;
		/// <summary>Normals and normal indices for the model.</summary>
		NormalData normal_data;
		/// <summary>Uvs and uv indices for the model.</summary>
		UVData uv_data;
	};

	/// <summary>
	/// Parse the given object file into an Object.
	/// </summary>
	/// <param name="filepath">
	/// Path to the object file to parse.
	/// </param>
	/// <returns>Parsed representation of the file contents.</returns>
	Object parse_object_file(czstring filepath);
}   // namespace glge::obj_parser
