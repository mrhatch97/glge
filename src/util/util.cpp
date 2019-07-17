#include "glge/util/util.h"

#include <algorithm>
#include <cerrno>
#include <iostream>
#include <string>

namespace glge::util
{
	void print_nested_exception(const std::exception & e, unsigned int level)
	{
		std::cerr << std::string(level, ' ') << "exception: " << e.what()
				  << '\n';
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

	UniqueHandle::UniqueHandle() : destroy(false) {}

	UniqueHandle::UniqueHandle(std::function<void()> enter, FunctionType exit) :
		destroy(true)
	{
		enter();
		exits.push(exit);
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

	void UniqueHandle::call_exits() noexcept
	{
		while (!exits.empty())
		{
			auto & exit_func = exits.top();
			exit_func();
			exits.pop();
		}
	}

	void UniqueHandle::reset()
	{
		call_exits();
		destroy = false;
	}

	void UniqueHandle::reset(FunctionType exit)
	{
		reset();
		exits.push(exit);
		destroy = true;
	}

	void UniqueHandle::release()
	{
		while (!exits.empty())
		{
			exits.pop();
		}
		destroy = false;
	}

	UniqueHandle & UniqueHandle::chain(std::function<void()> enter,
									   FunctionType exit)
	{
		if (!destroy)
		{
			throw std::logic_error("Cannot chain a unique handle with no exit "
								   "responsibility");
		}

		enter();
		exits.push(exit);
		return *this;
	}

	UniqueHandle::operator bool() { return destroy; }

	UniqueHandle::~UniqueHandle()
	{
		if (destroy)
		{
			call_exits();
		}
	}
}   // namespace glge::util
