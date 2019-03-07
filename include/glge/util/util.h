#pragma once

#include <glge/common.h>

#include <chrono>
#include <functional>
#include <tuple>
#include <sstream>
#include <stdexcept>
#include <string>

/// <summary>
/// Produces an exception message with extra source file information
/// in debug builds; otherwise, returns the unmodified exception message.
/// </summary>
#define EXC_MSG(msg) exception_message(msg, __FILE__, __LINE__, __func__)

/// <summary>
/// Produces an exception message with extra source file information
/// in debug builds; otherwise, returns the unmodified exception message. 
/// Helper for <c>EXC_MSG</c> macro.
/// </summary>
/// <returns><c>std::string</c> formatted exception message.</returns>
inline std::string exception_message(const std::string msg, czstring filename, std::size_t line, czstring func)
{
#if _DEBUG
	std::ostringstream ss;
	ss << msg << " (" << filename << ":" << line << "@" << func << ")";
	return ss.str();
#else
	return msg;
#endif
}

namespace glge::util
{
	using bench_clock = std::chrono::steady_clock;

	/// <summary>Tests whether type F returns void when invoked.</summary>
	template<typename F>
	using returns_void = std::is_same<std::invoke_result_t<F>, void>;

	/// <summary>Tests whether type F returns void when invoked.</summary>
	template<typename F>
	constexpr bool returns_void_v = returns_void<F>::value;

	template<typename F, typename... Args>
	std::conditional_t<
		returns_void_v<F>, 
		bench_clock::duration, 
		std::tuple<std::invoke_result_t<F>, bench_clock::duration>
	>
	time_op(const F & f, Args&&... args)
	{
		auto start = bench_clock::now();
		if constexpr (returns_void_v<F>)
		{
			f(std::forward<Args>(args)...);
			return bench_clock::now() - start;
		}
		else
		{
			auto result = f(std::forward<Args>(args)...);
			return { result, bench_clock::now() - start };
		}
	}

	// Based on https://stackoverflow.com/q/36834799
	/// <summary>
	/// Converts from one scalar type to another. 
	/// <para> If the conversion is widening, this is identical to 
	/// <c>static_cast</c>. If the conversion is narrowing, checks to see 
	/// that no data is lost and throws <c>std::logic_error</c> if the value changes. 
	/// </para>
	/// </summary>
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

	struct [[nodiscard]] UniqueHandle
	{
	private:
		bool destroy;
		std::function<void()> enter;
		std::function<void()> exit;

	public:
		UniqueHandle(std::function<void()> enter, std::function<void()> exit);

		UniqueHandle(const UniqueHandle &) = delete;
		UniqueHandle(UniqueHandle && other) noexcept;

		UniqueHandle & operator=(const UniqueHandle &) = delete;
		UniqueHandle & operator=(UniqueHandle && other) = delete;

		~UniqueHandle();
	};

	// Dynamically-allocated 2D array wrapper
	template<typename T>
	class Matrix
	{
	private:
		uptr<T[]> mat;
		const std::size_t _width;
		const std::size_t _height;

	public:
		Matrix(std::size_t width, std::size_t height) : mat(new T[width * height]()), _width(width), _height(height)
		{
		}

		Matrix(const Matrix &) = delete;
		Matrix(Matrix &&) = default;

		Matrix & operator=(const Matrix &) = delete;
		Matrix & operator=(Matrix &&) = default;

		[[nodiscard]] T & operator[](std::size_t idx)
		{
			return mat[idx];
		}

		[[nodiscard]] const T & operator[](std::size_t idx) const
		{
			return mat[idx];
		}

		[[nodiscard]] const T & at(std::size_t x, std::size_t y) const
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

		[[nodiscard]] T & at(std::size_t x, std::size_t y)
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

		[[nodiscard]] std::size_t index(std::size_t x, std::size_t y) const
		{
			return x + _width * y;
		}

		[[nodiscard]] std::size_t width() const
		{
			return _width;
		}

		[[nodiscard]] std::size_t height() const
		{
			return _height;
		}
	};

	void print_nested_exception(const std::exception & e, unsigned int level = 0);

	bool checked_fscanf(const int count, const char * buf, czstring fmt, ...);
}
