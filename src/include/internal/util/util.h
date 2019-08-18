/// <summary>Miscellaneous programming utilities.</summary>
///
/// Contains various utility classes and functions used by glge.
///
/// \file util.h

#pragma once

#include "glge/util/util.h"

#include <cstdio>
#include <chrono>
#include <stdexcept>

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

	/// <summary>
	/// Call the given function with the given arguments and return
	/// its value as well as the time it took to execute.
	/// </summary>
	/// Invokes the given callable f with the given args; times the operation
	/// and returns the result of f in a tuple with the duration the call took.
	/// If f returns void, returns the duration the call took directly.
	/// <param name="f">Callable object to invoke.</param>
	/// <param name="args">
	/// Optional parameter pack to pass to the invocation of f.
	/// </param>
	/// <typeparam name="F">Type of f. Must be callable.</typeparam>
	/// <typeparam name="Args">Parameter pack of args to pass to F.</typeparam>
	/// <returns>
	/// Tuple of (result, duration) or duration if result is void.
	/// </returns>
	template<typename F,
			 typename Clock = std::chrono::steady_clock,
			 typename... Args>
	std::conditional_t<
		returns_void_v<F>,
		typename Clock::duration,
		std::tuple<std::invoke_result_t<F>, typename Clock::duration>>
	time_op(const F & f, Args &&... args)
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
			return {result, Clock::now() - start};
		}
	}

	// Based on https://stackoverflow.com/q/36834799
	/// <summary>
	/// Converts from one scalar type to another, checking for data loss.
	/// </summary>
	/// Converts from From to To. If the conversion is widening, this is
	/// identical to <c>static_cast</c>. If the conversion is narrowing, checks
	/// to see that no data is lost and throws <c>std::logic_error</c> if the
	/// value
	/// changes.
	/// <param name="value">Value to convert from.</param>
	/// <typeparam name="From">
	/// Type of value to convert from. Must be scalar.
	/// </typeparam>
	/// <typeparam name="To">Type to convert to. Must be scalar.</typeparam>
	/// <exception cref="std::logic_error">
	/// Throws if narrowing conversion changes value.
	/// </exception>
	/// <returns>The converted value.</returns>
	template<typename From, typename To>
	std::enable_if_t<
		std::conjunction_v<std::is_scalar<From>, std::is_scalar<To>>,
		To>
	safe_cast(const From & value)
	{
		To new_value = static_cast<To>(value);

		if (value != static_cast<From>(new_value))
		{
			throw std::logic_error(EXC_MSG("Cast altered the target value"));
		}

		return new_value;
	}
}   // namespace glge::util
