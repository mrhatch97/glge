/// \brief Programming utilities.
///
/// Contains various utility classes and functions used by glge.
///
/// \file util.h

#pragma once

#include <glge/common.h>

#include <chrono>
#include <forward_list>
#include <functional>
#include <tuple>
#include <sstream>
#include <stdexcept>
#include <string>

/// <summary>Get an exception message formatted with extra debug info.</summary>
/// Produces an exception message with extra source file information
/// in debug builds; otherwise, returns the unmodified exception message.
/// <seealso cref="glge::util::exception_message"/>
/// <param name="msg">std::string to use as the main exception message.</param>
/// <returns>
/// Exception message with extra file info in debug configurations,
/// otherwise the original error.
/// </returns>
#define EXC_MSG(msg) glge::util::exception_message(msg, __FILE__, __LINE__, __func__)

namespace glge::util
{
	/// <summary>Tests whether type F returns void when invoked.</summary>
	/// <typeparam name="F">Callable type to test.</typeparam>
	template<typename F>
	using returns_void = std::is_same<std::invoke_result_t<F>, void>;

	/// <summary>Tests whether type F returns void when invoked.</summary>
	/// <typeparam name="F">Callable type to test.</typeparam>
	template<typename F>
	constexpr bool returns_void_v = returns_void<F>::value;

	/// <summary>Get an exception message formatted with extra debug info.</summary>
	/// Produces an exception message with extra source file information
	/// in debug builds; otherwise, returns the unmodified exception message.
	/// <seealso cref="EXC_MSG(msg)"/>
	/// <param name="msg">std::string to use as the main exception message.</param>
	/// <param name="filename">Name of the source file the exception originated from.</param>
	/// <param name="line">Line number the exception originated from.</param>
	/// <param name="func">Name of the function the exception originated from.</param>
	/// <returns><c>std::string</c> formatted exception message.</returns>
	inline string exception_message(const string msg, czstring filename, size_t line, czstring func)
	{
		if constexpr (debug)
		{
			std::ostringstream ss;
			ss << msg << " (" << filename << ":" << line << "@" << func << ")";
			return ss.str();
		}
		else
		{
			return msg;
		}
	}

	/// <summary>Pretty printer for nested std::exception.</summary>
	/// Prints the message in the given exception at the given indentation level;
	/// if e has sub-exceptions, these are also printed at increasing indentation
	/// levels.
	/// <param name="e">Exception to be printed.</param>
	/// <param name="level">Exception nesting level; should usually be defaulted
	/// by the caller.</param>
	void print_nested_exception(const std::exception & e, unsigned int level = 0);

	/// <summary>Call the given function with the given arguments and return
	/// its value as well as the time it took to execute.</summary>
	/// Invokes the given callable f with the given args; times the operation
	/// and returns the result of f in a tuple with the duration the call took.
	/// If f returns void, returns the duration the call took directly.
	/// <param name="f">Callable object to invoke.</param>
	/// <param name="args">Optional parameter pack to pass to the invocation of f.</param>
	/// <typeparam name="F">Type of f. Must be callable.</typeparam>
	/// <typeparam name="Args">Parameter pack of args to pass to F.</typeparam>
	/// <returns>Tuple of (result, duration) or duration if result is void.</returns>
	template<typename F, typename Clock = std::chrono::steady_clock, typename... Args>
	std::conditional_t<
		returns_void_v<F>,
		typename Clock::duration,
		std::tuple<std::invoke_result_t<F>, typename Clock::duration>
	>
	time_op(const F & f, Args&&... args)
	{
		auto start = Clock::now();
		if constexpr (returns_void_v<F>)
		{
			f(std::forward<Args>(args)...);
			return Clock::now() - start;
		}
		else
		{
			auto result = f(std::forward<Args>(args)...);
			return { result, Clock::now() - start };
		}
	}

	// Based on https://stackoverflow.com/q/36834799
	/// <summary>
	/// Converts from one scalar type to another, checking for data loss.
	/// </summary>
	/// Converts from From to To. If the conversion is widening, this is identical
	/// to <c>static_cast</c>. If the conversion is narrowing, checks to see
	/// that no data is lost and throws <c>std::logic_error</c> if the value changes.
	/// <param name="value">Value to convert from.</param>
	/// <typeparam name="From">Type of value to convert from. Must be scalar.</typeparam>
	/// <typeparam name="To">Type to convert to. Must be scalar.</typeparam>
	/// <exception cref="std::logic_error">
	/// Throws if narrowing conversion changes value.
	/// </exception>
	template<typename From, typename To>
	std::enable_if_t<
		std::conjunction_v<
			std::is_scalar<From>, std::is_scalar<To>
		>,
		To
	>
	safe_cast(const From & value)
	{
		To new_value = static_cast<To>(value);

		if (value != static_cast<From>(new_value))
		{
			throw std::logic_error(EXC_MSG("Cast altered the target value"));
		}

		return new_value;
	}

	/// <summary>Helper for lifting bracket pattern code into RAII code.</summary>
	/// Stores two functions with side effects. Entry function is run when the
	/// object is created, exit function when it is destroyed. This allows
	/// bracket pattern style code (e.g. resource acquisition and release)
	/// to be lifted into RAII code.
	struct [[nodiscard]] UniqueHandle
	{
	private:
		bool destroy;
		std::forward_list<std::function<void()>> exits;

		void call_exits() const;

	public:
		UniqueHandle();

		/// <summary>
		/// Construct a new UniqueHandle with the given entry and exit
		/// functions.
		/// </summary>
		///
		/// <param name="enter">Entry function; run immediately.</param>
		/// <param name="exit">Exit function; run on object destruction.</param>
		UniqueHandle(std::function<void()> enter, std::function<void()> exit);

		UniqueHandle(const UniqueHandle &) = delete;

		/// <summary>Construct a new UniqueHandle by moving an existing
		/// UniqueHandle's exit responsibility to the new object.</summary>
		///
		/// <param name="other">Other UniqueHandle to be moved from</param>
		UniqueHandle(UniqueHandle && other) noexcept;

		UniqueHandle & operator=(const UniqueHandle &) = delete;
		UniqueHandle & operator=(UniqueHandle && other);

		void reset();
		void reset(std::function<void()> exit);
		void release();
		UniqueHandle & chain(std::function<void()> enter, std::function<void()> exit);

		operator bool();

		/// <summary>Destructor calls the exit function if this UniqueHandle is
		/// still responsible for the call.</summary>
		~UniqueHandle();
	};

	/// <summary>Dynamically-sized 2D array wrapper.</summary>
	/// Helper for accessing a dynamically-allocated array with 2D indices.
	/// <typeparam name="T">Type to store in the Matrix. No requirements.</typeparam>
	template<typename T>
	class Matrix
	{
	private:
		unique_ptr<T[]> mat;
		const size_t _width;
		const size_t _height;

	public:
		Matrix(std::size_t width, size_t height) :
			mat(new T[width * height]()), _width(width), _height(height)
		{
		}

		Matrix(const Matrix &) = delete;
		Matrix(Matrix &&) = default;

		Matrix & operator=(const Matrix &) = delete;
		Matrix & operator=(Matrix &&) = default;

		/// <summary>Get a reference to the object in the Matrix at the given 1D
		/// index.</summary>
		/// <seealso cref="at()"/>
		/// <seealso cref="index()"/>
		/// <param name="idx">1D index into the Matrix. Moves across width, then height.</param>
		/// <returns>Reference to object at (x, y).</returns>
		[[nodiscard]] T & operator[](std::size_t idx)
		{
			return mat[idx];
		}

		/// <summary>Get a const reference to the object in the Matrix at the given 1D
		/// index.</summary>
		/// <seealso cref="at()"/>
		/// <seealso cref="index()"/>
		/// <param name="idx">1D index into the Matrix. Moves across width, then height.</param>
		/// <returns>Const reference to object at (x, y).</returns>
		[[nodiscard]] const T & operator[](size_t idx) const
		{
			return mat[idx];
		}

		/// <summary>
		/// Get a const reference to the object in the Matrix at the given 2D index.
		/// </summary>
		/// <param name="x">x-coordinate; moves along the width of the Matrix.</param>
		/// <param name="y">y-coordinate; moves along the height of the Matrix.</param>
		/// <returns>Const reference to object at (x, y).</returns>
		/// <exception cref="std::out_of_range">Throws if given indices are outside
		/// the bounds of the Matrix.</exception>
		[[nodiscard]] const T & at(size_t x, size_t y) const
		{
			if (x < 0 || x >= _width)
			{
				throw std::out_of_range(EXC_MSG("Matrix indices out of bounds"));
			}

			if (y < 0 || y >= _height)
			{
				throw std::out_of_range(EXC_MSG("Matrix indices out of bounds"));
			}

			return this->operator[](index(x, y));
		}

		/// <summary>
		/// Get a reference to the object in the Matrix at the given 2D index.
		/// </summary>
		/// <param name="x">x-coordinate; moves along the width of the Matrix.</param>
		/// <param name="y">y-coordinate; moves along the height of the Matrix.</param>
		/// <returns>Reference to object at (x, y).</returns>
		/// <exception cref="std::out_of_range">Throws if given indices are outside
		/// the bounds of the Matrix.</exception>
		[[nodiscard]] T & at(size_t x, size_t y)
		{
			if (x < 0 || x >= _width)
			{
				throw std::out_of_range(EXC_MSG("Matrix indices out of bounds"));
			}

			if (y < 0 || y >= _height)
			{
				throw std::out_of_range(EXC_MSG("Matrix indices out of bounds"));
			}

			return (*this)[index(x, y)];
		}

		/// <summary>Convert from 2D indices to 1D index for access.</summary>
		/// <param name="x">x-coordinate; moves along the width of the Matrix.</param>
		/// <param name="y">y-coordinate; moves along the height of the Matrix.</param>
		/// <seealso cref="operator[]()"/>
		/// <returns>1D index into this Matrix.</returns>
		[[nodiscard]] size_t index(size_t x, size_t y) const
		{
			return x + _width * y;
		}

		/// <summary>Get the width of the Matrix.</summary>
		/// <returns>Width of the Matrix.</returns>
		[[nodiscard]] size_t width() const
		{
			return _width;
		}

		/// <summary>Get the height of the Matrix.</summary>
		/// <returns>Height of the Matrix.</returns>
		[[nodiscard]] size_t height() const
		{
			return _height;
		}
	};

	/// <summary>Wrapper for fscanf IO that checks whether the count of
	/// matched variables equals the expected count.</summary>
	/// Runs scanf pattern match on the given buffer with the given format
	/// string and optional arguments. Return value indicates whether expected
	/// count matches actual count. Use FMT_STRING_ARG(buf) for MSVC compatibility.
	/// <param name="count">Expected number of matched format variables.</param>
	/// <param name="buf">Pointer to character buffer to read from.</param>
	/// <param name="fmt">C IO format string.</param>
	/// <param name="...">Variadic arguments for C IO format string.</param>
	bool checked_sscanf(int count, const char * buf, czstring fmt, ...);
}
