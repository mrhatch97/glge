#include "glge/util/util.h"

#include <algorithm>
#include <cerrno>
#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <string>

namespace glge
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

		bool checked_sscanf(int count, const char * buf, czstring fmt, ...)
		{
			va_list ap;

			va_start(ap, fmt);

#ifdef _MSC_VER
			int matchCt = vsscanf_s(buf, fmt, ap);
#else
			int matchCt = vsscanf(buf, fmt, ap);
#endif

			va_end(ap);

			return count == matchCt;
		}

		UniqueHandle::UniqueHandle() : destroy(false)
		{
		}

		UniqueHandle::UniqueHandle(std::function<void()> enter, std::function<void()> exit)
			: destroy(true)
		{
			enter();
			exits.push_front(exit);
		}

		UniqueHandle::UniqueHandle(UniqueHandle && other) noexcept :
			destroy(other.destroy), exits(std::move(other.exits))
		{
			other.destroy = false;
		}

		UniqueHandle & UniqueHandle::operator=(UniqueHandle && other)
		{
			exits = std::move(other.exits);
			destroy = other.destroy;
			other.destroy = false;

			return *this;
		}

		void UniqueHandle::call_exits() const
		{
			std::for_each(exits.cbegin(), exits.cend(), [](std::function<void()> exit) { exit(); });
		}

		void UniqueHandle::reset()
		{
			call_exits();
			exits.clear();
			destroy = false;
		}

		void UniqueHandle::reset(std::function<void()> exit)
		{
			reset();
			exits.push_front(exit);
			destroy = true;
		}

		void UniqueHandle::release()
		{
			exits.clear();
			destroy = false;
		}

		UniqueHandle & UniqueHandle::chain(std::function<void()> enter, std::function<void()> exit)
		{
			if (!destroy)
			{
				throw std::logic_error("Cannot chain a unique handle with no exit responsibility");
			}

			enter();
			exits.push_front(exit);
			return *this;
		}

		UniqueHandle::operator bool()
		{
			return destroy;
		}

		UniqueHandle::~UniqueHandle()
		{
			if (destroy)
			{
				call_exits();
			}
		}
	}
}
