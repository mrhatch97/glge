/// <summary>Support for cross-platform capability.</summary>
/// /// Contains utility macros and variables for cross compatibility between
/// Windows, Linux, and OSX.
///
/// \file compat.h

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
	constexpr OperatingSystem OperatingSystem = OperatingSystem::Windows;
#elif __linux__
	constexpr OperatingSystem OperatingSystem = OperatingSystem::Linux;
#elif __APPLE__
	constexpr OperatingSystem OperatingSystem = OperatingSystem::OSX;
#else
#error "Could not identify platform for compatibility"
#endif

	constexpr bool OnWindows = OperatingSystem == OperatingSystem::Windows;
	constexpr bool OnLinux = OperatingSystem == OperatingSystem::Linux;
	constexpr bool OnOSX = OperatingSystem == OperatingSystem::OSX;
}   // namespace glge

/// \def FMT_STRING_ARG(buf)
/// <summary>Helper for format string arguments.</summary>
///
/// Supports passing same format string argument to standard C IO functions and
/// MSVC _s variant by automatically adding sizeof(buf) when compiled under
/// MSVC.
/// <param name="buf">Buffer used to store the read format string value.</param>
#ifdef _MSC_VER
#define FMT_STRING_ARG(buf) buf, sizeof(buf)
#else
#define FMT_STRING_ARG(buf) buf
#endif

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
	/// Attempt to open the given file path in the given mode.
	/// If this fails for any reason, throws an exception.
	/// </summary>
	/// <param name="file_name">Path to file to open.</param>
	/// <param name="mode">Mode string to open file with; as in fopen.</param>
	/// <returns>Handle to opened file.</returns>
	/// <exception cref="std::runtime_error">
	/// Thrown if file fails to open.
	/// </exception>
	inline FILE * try_fopen(czstring file_name, czstring mode)
	{
		FILE * file;

#ifdef _MSC_VER
		if (fopen_s(&file, file_name, mode) != 0)
#else
		errno = 0;

		file = std::fopen(file_name, mode);

		if (errno)
#endif
		{
			throw std::runtime_error("Failed to open file " +
									 string(file_name));
		}

		return file;
	}

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

#if OnWindows
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
