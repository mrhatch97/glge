#pragma once

#define GLM_ENABLE_EXPERIMENTAL

// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>
#include <vector>

extern std::mutex stdio_write_lock;

using zstring = char *;
using czstring = const char *;

namespace eng
{
	template<typename T>
	using uptr = std::unique_ptr<T>;

	template<typename T>
	using shptr = std::shared_ptr<T>;

	template<typename T>
	using Vector = std::vector<T>;

	template<typename K, typename V>
	using Map = std::map<K, V>;

	using String = std::string;

	using size_t = std::size_t;

	//template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	//template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

	using vec2 = glm::vec2;
	using vec3 = glm::vec3;
	using vec4 = glm::vec4;
	using mat4 = glm::mat4;
	using quat = glm::quat;

	namespace util {}
	namespace math {}
	namespace input {}
	namespace procgen {}
	namespace render
	{
		namespace res {}
		namespace primitive {}
		namespace scene { using frame_clock = std::chrono::steady_clock; }
	}
}
