#pragma once

#include <glge/common.h>

#if _WIN32
#define GLGE_WINDOWS 1
#define GLGE_LINUX   0
#define GLGE_APPLE   0
#elif __linux__
#define GLGE_WINDOWS 0
#define GLGE_LINUX   1
#define GLGE_APPLE   0
#elif __APPLE__ 
#define GLGE_WINDOWS 0
#define GLGE_LINUX   0
#define GLGE_APPLE   1
#else
#error "Could not identify platform for compatibility"
#endif

// Format string argument for _s family of C IO functions - MSVC only
#ifdef _MSC_VER
#define FMT_STRING_ARG(buf) buf, sizeof(buf)
#else
#define FMT_STRING_ARG(buf) buf
#endif

// Use execution policy if available
#if __has_include(<execution>)

#include <execution>

#define EXECUTION_POLICY_SEQ       std::execution::seq, 
#define EXECUTION_POLICY_PAR       std::execution::par, 
#define EXECUTION_POLICY_PAR_UNSEQ std::execution::par_unseq, 

#else

#define EXECUTION_POLICY_SEQ
#define EXECUTION_POLICY_PAR
#define EXECUTION_POLICY_PAR_UNSEQ

#endif

// Use std::filesystem for current working directory if available, else fallback to platform-specific get_cwd
#if __has_include(<filesystem>)

#include <filesystem>

#define GET_CWD() std::filesystem::current_path().string()

#else

#define GET_CWD() getcwd_safe()

#if GLGE_WINDOWS
#include <direct.h>
#define getcwd _getcwd 
#else
#include <unistd.h>
#endif

inline std::string getcwd_safe()
{
	char buf[256];
	czstring cwd = getcwd(buf, sizeof(buf));

	if (cwd)
	{
		std::string res = std::string(cwd);
		return res;
	}
	else
	{
		return std::string("");
	}
}

#endif