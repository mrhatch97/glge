/// <summary>Support for cross-platform capability.</summary>
/// /// Contains utility macros and variables for cross compatibility between
/// Windows, Linux, and OSX.
///
/// \file _compat.h

#pragma once

#include <glge/common.h>

namespace glge
{
	enum class OperatingSystem
	{
		Windows,
		Linux,
		OSX
	};

#if _WIN32
	constexpr OperatingSystem operating_system = OperatingSystem::Windows;
#elif __linux__
	constexpr OperatingSystem operating_system = OperatingSystem::Linux;
#elif __APPLE__
	constexpr OperatingSystem operating_system = OperatingSystem::OSX;
#else
#error "Could not identify platform for compatibility"
#endif

	constexpr bool on_windows = operating_system == OperatingSystem::Windows;
	constexpr bool on_linux = operating_system == OperatingSystem::Linux;
	constexpr bool on_osx = operating_system == OperatingSystem::OSX;
}   // namespace glge

/// \def EXECUTION_POLICY_SEQ
/// <summary>
/// Function argument equivalent to std::execution::seq. Do not follow with 
/// comma.
/// </summary>
/// Allows use of sequential execution policy, automatically excluded on
/// systems that do not support execution policies.

/// \def EXECUTION_POLICY_PAR
/// <summary>
/// Function argument equivalent to std::execution::par. Do not follow with 
/// comma.
/// </summary>
/// Allows use of parallel execution policy, automatically excluded on
/// systems that do not support execution policies.

/// \def EXECUTION_POLICY_PAR_UNSEQ
/// <summary>
/// Function argument equivalent to std::execution::par_unseq. Do not follow with 
/// comma.
/// </summary>
/// Allows use of unsequenced parallel execution policy, automatically excluded
/// on systems that do not support execution policies.

#if __has_include(<execution>)

#include <execution>

#define EXECUTION_POLICY_SEQ std::execution::seq,
#define EXECUTION_POLICY_PAR std::execution::par,
#define EXECUTION_POLICY_PAR_UNSEQ std::execution::par_unseq,

#else

#define EXECUTION_POLICY_SEQ
#define EXECUTION_POLICY_PAR
#define EXECUTION_POLICY_PAR_UNSEQ

#endif

namespace glge::util
{
	/// <summary>
	/// Platform-independent function for getting the current working directory.
	/// </summary>
	///
	/// Gets the current working directory. Prefers std::filesystem if
	/// available, falls back to platform specific cwd functions if not.
	/// <returns>
	/// Current working directory, as a string, or empty string if retrieval
	/// failed.
	/// </returns>
	string getcwd();
}   // namespace glge::util

#if __has_include(<filesystem>)

#include <filesystem>

namespace glge::util
{
	string getcwd() { return std::filesystem::current_path().string(); }
}   // namespace glge::util

#else

#if _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

namespace glge::util
{
	string getcwd()
	{
		char buf[256];
		czstring cwd = getcwd(buf, sizeof(buf));

		if (cwd)
		{
			string res = string(cwd);
			return res;
		}
		else
		{
			return string("");
		}
	}
}   // namespace glge::util
#endif
