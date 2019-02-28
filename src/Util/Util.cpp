#include "glge/Util/Util.h"

#include <cerrno>
#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <string>

namespace eng
{
	namespace util
	{
		void print_nested_exception(const std::exception & e, unsigned int level)
		{
			std::cerr << std::string(level, ' ') << "exception: " << e.what() << '\n';
			try 
			{
				std::rethrow_if_nested(e);
			}
			catch (const std::exception & e) 
			{
				print_nested_exception(e, level + 1);
			}
			catch (...)
			{
				std::cerr << "Unknown object thrown" << std::endl;
			}
		}

		bool checked_fscanf(const int count, const char * buf, czstring fmt, ...)
		{
			va_list ap;

			va_start(ap, fmt);

#ifdef _MSC_VER
			int matchCt = vsscanf_s(buf, fmt, ap);
#else
			int matchCt = std::vsscanf(buf, fmt, ap);
#endif
			va_end(ap);

			return count == matchCt;
		}

		UniqueHandle::UniqueHandle(std::function<void()> enter, std::function<void()> exit)
			: destroy(true), enter(enter), exit(exit)
		{
			enter();
		}

		UniqueHandle::UniqueHandle(UniqueHandle && other) noexcept : 
			destroy(other.destroy), enter(other.enter), exit(other.exit)
		{
			other.destroy = false;
		}

		UniqueHandle::~UniqueHandle()
		{
			if (destroy)
			{
				exit();
			}
		}
	}
}
