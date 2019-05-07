/// <summary>Utilities for converting heightmaps to 3D models.</summary>
///
/// \file heightmap.h

#pragma once

#include <glge/common.h>

namespace glge::util
{
	/// <summary>
	/// Converts a heightmap with the given dimensions to a list of
	/// indices describing the heightmap as a mesh of triangles.
	/// </summary>
	/// <param name="width">
	/// Width of the heightmap.
	/// </param>
	/// <param name="height">
	/// Height of the heightmap.
	/// </param>
	/// <returns>Vector of indices.</returns>
	vector<unsigned int> heightmap_indices(size_t width, size_t height);

	/// <summary>
	/// Computes the normal vector for each triangle in a heightmap mesh.
	/// </summary>
	/// <param name="heightmap">
	/// Heightmap to compute normals for.
	/// </param>
	/// <param name="indices">
	/// List of indices representing the heightmap as a mesh of triangles.
	/// </param>
	/// <returns>Vector of normals.</returns>
	vector<vec3> heightmap_normals(const vector<vec3> & heightmap,
								   const vector<unsigned int> & indices);

	/// <summary>
	/// Computes the texture coordinates (uvs) for each vertex in a heightmap
	/// mesh.
	/// </summary>
	/// <param name="width">
	/// Width of the heightmap.
	/// </param>
	/// <param name="height">
	/// Height of the heightmap.
	/// </param>
	/// <returns>Vector of uvs.</returns>
	vector<vec2> heightmap_uvs(size_t width, size_t height);
}   // namespace glge::util
