/// <summary>Miscellaneous programming utilities.</summary>
///
/// Contains various utility classes and functions used by glge.
///
/// \file util.h

#pragma once

#include <glge/common.h>

#include <functional>
#include <iostream>
#include <ostream>
#include <stack>

/// <summary>Get an exception message formatted with extra debug info.</summary>
/// Produces an exception message with extra source file information
/// in debug builds; otherwise, returns the unmodified exception message.
/// <seealso cref="glge::util::exception_message"/>
/// <param name="msg">std::string to use as the main exception message.</param>
/// <returns>
/// Exception message with extra file info in debug configurations,
/// otherwise the original error.
/// </returns>
#define EXC_MSG(msg) \
	glge::util::exception_message(msg, __FILE__, __LINE__, __func__)

namespace glge::util
{
	/// <summary>
	/// Get an exception message formatted with extra debug info.
	/// </summary>
	/// Produces an exception message with extra source file information
	/// in debug builds; otherwise, returns the unmodified exception message.
	/// <seealso cref="EXC_MSG(msg)"/>
	/// <param name="msg">
	/// std::string to use as the main exception message.
	/// </param>
	/// <param name="filename">
	/// Name of the source file the exception originated from.
	/// </param>
	/// <param name="line">Line number the exception originated from.</param>
	/// <param name="func">
	/// Name of the function the exception originated from.
	/// </param>
	/// <returns>
	/// <c>std::string</c> formatted exception message.
	/// </returns>
	string exception_message(string msg,
							 czstring filename,
							 size_t line,
							 czstring func);

	/// <summary>Pretty printer for nested std::exception.</summary>
	/// Prints the message in the given exception at the given indentation
	/// level; if e has sub-exceptions, these are also printed at increasing
	/// indentation levels.
	/// <param name="e">Exception to be printed.</param>
	/// <param name="level">
	/// Exception nesting level; should usually be defaulted by the caller.
	/// </param>
	/// <param name="stream">
	/// Stream to print to. Defaults to std::cerr.
	/// </param>
	void print_nested_exception(const std::exception & e,
								unsigned int level = 0,
								std::ostream & stream = std::cerr);

	/// <summary>
	/// Force a conversion from Parent to Child type without
	/// checking type constraints.
	/// </summary>
	/// <param name="value">
	/// Value to treat as the Child type.
	/// </param>
	/// <typeparam name="Child">Type to convert to.</typeparam>
	/// <typeparam name="Parent">Type to convert from.</typeparam>
	/// <returns>The converted subtype value.</returns>
	template<typename Child, typename Parent>
	constexpr Child unchecked_conversion(Parent value);

	/// <summary>
	/// A type that can be inherited from to create a strong typedef of
	/// type that would not normally allow inheritance, e.g. primitives.
	/// </summary>
	/// <typeparam name="Child">The new type to create.</typeparam>
	/// <typeparam name="Parent">The type to create a subtype of.</typeparam>
	template<typename Child, typename Parent>
	class Newtype
	{
		Parent value;

	protected:
		/// <summary>
		/// Default constructor. Required to allow unchecked conversion.
		/// </summary>
		constexpr Newtype() noexcept = default;

		/// <summary>
		/// Explicitly create an instance of the subtype from the supertype.
		/// To be invoked by child classes after subtype constraints are
		/// checked.
		/// </summary>
		/// <param name="value">Value to initialize newtype with.</param>
		explicit constexpr Newtype(Parent value) noexcept : value(value) {}

		/// <summary>
		/// Default copy constructor to allow copying of newtypes.
		/// </summary>
		/// <param name="other">Value to copy.</param>
		constexpr Newtype(const Newtype & other) = default;

	public:
		/// <summary>
		/// Widening conversion from subtype to supertype. Implicitly allowed.
		/// </summary>
		/// <returns>Reference to this value as its parent type.</returns>
		constexpr operator Parent &() noexcept { return value; }

		/// <summary>
		/// Widening conversion from subtype to supertype. Implicitly allowed.
		/// </summary>
		/// <returns>Const reference to this value as its parent type.</returns>
		constexpr operator const Parent &() const noexcept { return value; }

		friend constexpr Child
		unchecked_conversion<Child, Parent>(Parent value);
	};

	template<typename Child, typename Parent>
	constexpr Child unchecked_conversion(Parent value)
	{
		Child child;
		child.value = value;
		return child;
	}

	/// <summary>
	/// A specialized newtype for types with ordering. Allows
	/// enforcing a range constraint at compile-time on values
	/// of a type.
	/// </summary>
	/// <typeparam name="Child">
	/// The new range-constrained type to create.
	/// </typeparam>
	/// <typeparam name="Parent">
	/// The supertype to constrain.
	/// </typeparam>
	/// <typeparam name="lower_bound">
	/// The minimum (inclusive) value to allow for the range.
	/// </typeparam>
	/// <typeparam name="upper_bound">
	/// The maximum (inclusive) value to allow for the range.
	/// </typeparam>
	template<typename Child,
			 typename Parent,
			 Parent lower_bound,
			 Parent upper_bound>
	class Range : public Newtype<Child, Parent>
	{
	public:
		/// <summary>
		/// The minimum (inclusive) value allowed for this range.
		/// </summary>
		static constexpr Parent min = lower_bound;
		/// <summary>
		/// The maximum (inclusive) value allowed for this range.
		/// </summary>
		static constexpr Parent max = upper_bound;

	protected:
		/// <summary>
		/// Default constructor. Required to allow unchecked conversion.
		/// </summary>
		constexpr Range() noexcept {}

		/// <summary>
		/// Explicitly create an instance of the subtype from the supertype.
		/// To be invoked by child classes after subtype constraints are
		/// checked.
		/// </summary>
		/// <param name="value">Value to initialize newtype with.</param>
		explicit constexpr Range(Parent value) : Newtype<Child, Parent>(value)
		{
			if (value < lower_bound)
			{
				throw std::range_error("value was below the allowed range");
			}
			else if (value > upper_bound)
			{
				throw std::range_error("value was above the allowed range");
			}
		}

		/// <summary>
		/// Default copy constructor to allow copying of ranges.
		/// </summary>
		/// <param name="other">Value to copy.</param>
		constexpr Range(const Range & other) = default;
	};

	/// <summary>
	/// Helper for lifting bracket pattern code into RAII code.
	/// </summary>
	/// This handle stores a list of functions to be invoked for their side
	/// effects when the handle is destroyed. Exit functions are invoked in
	/// FIFO order to simulate "unwinding". Functions are invoked from the
	/// destructor and so should be noexcept.
	/// This allows bracket pattern style code (e.g. resource acquisition
	/// and release) to be lifted into RAII code.
	struct [[nodiscard]] UniqueHandle
	{
	public:
		// TODO switch to std::function<void() noexcept> when implemented by
		// compilers
		/// <summary>Type of exit functions.</summary>
		using FunctionType = std::function<void()>;

	private:
		bool destroy;
		std::stack<FunctionType> exits;

		void call_exits() noexcept;

	public:
		/// <summary>
		/// Construct a new UniqueHandle with no responsibilities.
		/// </summary>
		UniqueHandle();

		/// <summary>
		/// Construct a new UniqueHandle with the given entry and exit
		/// functions.
		/// </summary>
		/// <param name="enter">Entry function; run immediately.</param>
		/// <param name="exit">Exit function; run on object destruction.</param>
		UniqueHandle(std::function<void()> enter, FunctionType exit);

		/// <summary>
		/// Construct a new UniqueHandle by copying another handle.
		/// Deleted.
		/// </summary>
		/// <param name="other">
		/// Handle to copy from.
		/// </param>
		UniqueHandle(const UniqueHandle & other) = delete;

		/// <summary>
		/// Construct a new UniqueHandle by moving an existing
		/// UniqueHandle's exit responsibility to the new object.
		/// </summary>
		/// <param name="other">Other UniqueHandle to be moved from</param>
		UniqueHandle(UniqueHandle && other) noexcept;

		/// <summary>
		/// Copy another UniqueHandle into this handle. Deleted.
		/// </summary>
		/// <param name="other">
		/// Handle to copy from.
		/// </param>
		/// <returns>Reference to the copied-to handle.</returns>
		UniqueHandle & operator=(const UniqueHandle & other) = delete;

		/// <summary>
		/// Move the exit responsibilities of another UniqueHandle into this
		/// handle.
		/// </summary>
		/// The exit responsibilities of the other handle are
		/// moved into this handle. The other handle will have no more exit
		/// responsibilities.
		/// <param name="other">
		/// The UniqueHandle to move responsibility from.
		/// </param>
		/// <returns>Reference to the moved-to handle.</returns>
		UniqueHandle & operator=(UniqueHandle && other);

		/// <summary>
		/// Force all exit functions to be invoked and clear all
		/// responsibilities of the handle.
		/// </summary>
		void reset();

		/// <summary>
		/// Force all exit functions to be invoked, then remove all exit
		/// responsibilities and replace them with the given exit function.
		/// </summary>
		/// <param name="exit">
		/// New exit function for UniqueHandle to call.
		/// </param>
		void reset(FunctionType exit);

		/// <summary>
		/// Release all exit function responsbilities from this handle.
		/// </summary>
		void release();

		/// <summary>
		/// Chain a new bracket pattern into this handle.
		/// </summary>
		/// Assume a new responsibility for this handle. The entry function is
		/// invoked immediately, and the exit function is run at object
		/// destruction, before previously-specified exit functions.
		///
		/// <param name="enter">Entry function; run immediately.</param>
		/// <param name="exit">Exit function; run on object destruction.</param>
		/// <returns>Reference to the target of this invocation.</returns>
		UniqueHandle & chain(std::function<void()> enter, FunctionType exit);

		/// <summary>
		/// Checks if this handle has exit responsibilities>
		/// </summary>
		/// <returns>True if will invoke functions on destruction.</returns>
		operator bool();

		/// <summary>Destructor calls the exit functions if this UniqueHandle is
		/// still responsible for the calls.</summary>
		~UniqueHandle();
	};

	/// <summary>Dynamically-sized 2D array wrapper.</summary>
	/// Helper for accessing a dynamically-allocated array with 2D indices.
	/// <typeparam name="T">
	/// Type to store in the Matrix. No requirements.
	/// </typeparam>
	template<typename T>
	class Matrix
	{
	private:
		unique_ptr<T[]> mat;
		const size_t _width;
		const size_t _height;

	public:
		/// <summary>
		/// Construct a new matrix with the given dimensions.
		/// </summary>
		/// <param name="width">Width of the matrix.</param>
		/// <param name="height">Height of the matrix.</param>
		Matrix(std::size_t width, size_t height) :
			mat(new T[width * height]()), _width(width), _height(height)
		{}

		/// <summary>
		/// Construct a new Matrix by copying the given Matrix. Deleted.
		/// </summary>
		/// <param name="other">
		/// Matrix to copy from.
		/// </param>
		Matrix(const Matrix & other) = delete;

		/// <summary>
		/// Construct a new Matrix by moving the data from
		/// another Matrix.
		/// </summary>
		/// <param name="other">
		/// Matrix to move from.
		/// </param>
		Matrix(Matrix && other) = default;

		/// <summary>
		/// Copy the data from another Matrix into this Matrix.
		/// Deleted.
		/// </summary>
		/// <param name="other">
		/// Matrix to copy from.
		/// </param>
		/// <returns>Reference to the copied-to Matrix.</returns>
		Matrix & operator=(const Matrix & other) = delete;

		/// <summary>
		/// Move another Matrix into this Matrix.
		/// </summary>
		/// <param name="other">
		/// Matrix to move from.
		/// </param>
		/// <returns>Reference to the moved-to Matrix.</returns>
		Matrix & operator=(Matrix && other) = default;

		/// <summary>
		/// Get a reference to the object in the Matrix at the given 1D
		/// index.
		/// </summary>
		/// <seealso cref="at()"/>
		/// <seealso cref="index()"/>
		/// <param name="idx">
		/// 1D index into the Matrix. Moves across width, then height.
		/// </param>
		/// <returns>Reference to object at (x, y).</returns>
		[[nodiscard]] T & operator[](std::size_t idx) { return mat[idx]; }

		/// <summary>
		/// Get a const reference to the object in the Matrix at the given 1D
		/// index.
		/// </summary>
		/// <seealso cref="at()"/>
		/// <seealso cref="index()"/>
		/// <param name="idx">
		/// 1D index into the Matrix. Moves across width, then height.
		/// </param>
		/// <returns>Const reference to object at (x, y).</returns>
		[[nodiscard]] const T & operator[](size_t idx) const
		{
			return mat[idx];
		}

		/// <summary>
		/// Get a const reference to the object in the Matrix at the given 2D
		/// index.
		/// </summary>
		/// <param name="x">
		/// x-coordinate; moves along the width of the Matrix.
		/// </param>
		/// <param name="y">
		/// y-coordinate; moves along the height of the Matrix.
		/// </param>
		/// <returns>Const reference to object at (x, y).
		/// </returns>
		/// <exception cref="std::out_of_range">
		/// Throws if given indices are outside the bounds of the Matrix.
		/// </exception>
		[[nodiscard]] const T & at(size_t x, size_t y) const
		{
			if (x < 0 || x >= _width)
			{
				throw std::out_of_range(
					EXC_MSG("Matrix indices out of bounds"));
			}

			if (y < 0 || y >= _height)
			{
				throw std::out_of_range(
					EXC_MSG("Matrix indices out of bounds"));
			}

			return this->operator[](index(x, y));
		}

		/// <summary>
		/// Get a reference to the object in the Matrix at the given 2D index.
		/// </summary>
		/// <param name="x">
		/// x-coordinate; moves along the width of the Matrix.
		/// </param>
		/// <param name="y">
		/// y-coordinate; moves along the height of the Matrix.
		/// </param>
		/// <returns>Reference to object at (x, y).
		/// </returns>
		/// <exception cref="std::out_of_range">
		/// Throws if given indices are outside the bounds of the Matrix.
		/// </exception>
		[[nodiscard]] T & at(size_t x, size_t y)
		{
			if (x < 0 || x >= _width)
			{
				throw std::out_of_range(
					EXC_MSG("Matrix indices out of bounds"));
			}

			if (y < 0 || y >= _height)
			{
				throw std::out_of_range(
					EXC_MSG("Matrix indices out of bounds"));
			}

			return (*this)[index(x, y)];
		}

		/// <summary>Convert from 2D indices to 1D index for access.</summary>
		/// <param name="x">
		/// x-coordinate; moves along the width of the Matrix.
		/// </param>
		/// <param name="y">
		/// y-coordinate; moves along the height of the Matrix.
		/// </param>
		/// <seealso cref="operator[]()"/>
		/// <returns>1D index into this Matrix.</returns>
		[[nodiscard]] size_t index(size_t x, size_t y) const
		{
			return x + _width * y;
		}

		/// <summary>Get the width of the Matrix.</summary>
		/// <returns>Width of the Matrix.</returns>
		[[nodiscard]] size_t width() const { return _width; }

		/// <summary>Get the height of the Matrix.</summary>
		/// <returns>Height of the Matrix.</returns>
		[[nodiscard]] size_t height() const { return _height; }
	};
}   // namespace glge::util
