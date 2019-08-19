/// <summary>Common declarations and aliases for glge.</summary>
///
/// Definitions of main library namespaces, commonly used type aliases,
/// and other utilities.
///
/// \file common.h

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/mat4x4.hpp>

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

// Requires GLM 0.9.9.x - not always supported
#if 0
extern template struct glm::vec<2, float, glm::packed_highp>;
extern template struct glm::vec<3, float, glm::packed_highp>;
extern template struct glm::vec<4, float, glm::packed_highp>;
extern template struct glm::mat<4, 4, float, glm::packed_highp>;
extern template struct glm::qua<float, glm::packed_highp>;

extern template float glm::dot<>(const vec3 &, const vec3 &);
extern template float glm::radians<>(float);
#endif

/// <summary>Main library namespace.</summary>
/// Namespace for all components of the glge library.
namespace glge
{
	/// <summary>Mutex for synchronizing writes to stdout and stderr</summary>
	extern std::mutex stdio_write_lock;

#ifdef GLGE_DEBUG
	/// <summary>Flag for debug builds.</summary>
	constexpr bool debug = true;
#else
	/// <summary>Flag for debug builds.</summary>
	constexpr bool debug = false;
#endif

	/// <summary>
	/// Vocabulary type alias for null-terminated C strings.
	/// </summary>
	using zstring = char *;
	/// <summary>
	/// Vocabulary type alias for const null-terminated C strings.
	/// </summary>
	using czstring = const char *;

	// Extremely commonly used types
	using glm::mat4;
	using glm::quat;
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	using std::map;
	using std::size_t;
	using std::string;
	using std::unique_ptr;
	using std::vector;

	// TODO switch to std::observer_ptr when standardized
	/// <summary>Vocabulary type alias for non-owning pointers.</summary>
	template<typename W>
	using observer_ptr = W *;

	/// <summary>
	/// Helper struct for pattern-matching like std::visit syntax.
	/// </summary>
	/// A helper struct for std::visit to enable pattern matching-like syntax by
	/// aggregating lambdas in a single object.
	template<class... Ts>
	struct overloaded : Ts...
	{
		using Ts::operator()...;
	};

	/// <summary>
	/// Template deduction guide for std::visit pattern matching struct.
	/// </summary>
	/// User defined deduction guide for overloaded to allow implicit type
	/// deduction in pattern matching syntax.
	template<class... Ts>
	overloaded(Ts...)->overloaded<Ts...>;

	/// <summary>Namespace for programming utilities.</summary>
	/// Namespace containing various utility classes and functions used by glge.
	namespace util
	{}

	/// <summary>Namespace for mathematical utilities.</summary>
	/// Namespace containing mathematical helper classes and functions.
	namespace math
	{}

	/// <summary>Namespace for model file operations.</summary>
	/// Namespace containing functions for loading model files.
	namespace model_parser
	{}

	/// <summary>Namespace for event-based message passing.</summary>
	/// Namespace containing support for raising and handling events.
	namespace event
	{
		/// <summary>Namespace for user input handling.</summary>
		/// Namespace containing support for receiving and responding to user
		/// input.
		namespace input
		{}
	}   // namespace event

	/// <summary>Namespace for procedural generation functions.</summary>
	/// Namespace containing functions for procedural generation of heightmaps
	/// and other entities.
	namespace proc_gen
	{}

	/// <summary>Namespace for classes supporting 3D rendering.</summary>
	/// Namespace containing classes for describing a 3D scene in a scene graph,
	/// instantiating rendering primitives, and dispatching primitives to be
	/// drawn by the rendering backend.
	namespace renderer
	{
		/// <summary>Namespace for rendering primitive classes.</summary>
		/// Namespace containing classes for managing rendering primitives at
		/// an implementation-independent level.
		namespace primitive
		{}

		/// <summary>
		/// Namespace for 3D scene description as a scene graph.
		/// </summary>
		/// Namespace containing classes for describing a 3D scene as a scene
		/// graph, traversing it to produce rendering jobs, and manipulating
		/// rendering parameters of the scene.
		namespace scene_graph
		{}
	}   // namespace renderer

	/// <summary>Namespace for glge tests.</summary>
	/// Namespace for all tests for the glge library.
	namespace test
	{
		/// <summary>Namespace for glge test cases.</summary>
		/// Namespace for all general test cases for the glge library.
		namespace cases
		{}

		/// <summary>Namespace for glge OpenGL tests.</summary>
		/// Namespace for all OpenGL tests for the glge library.
		namespace opengl
		{
			/// <summary>Namespace for glge OpenGL test cases.</summary>
			/// Namespace for all OpenGL test cases for the glge library.
			namespace cases
			{}
		}   // namespace opengl
	}       // namespace test
}   // namespace glge
