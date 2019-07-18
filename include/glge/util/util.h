/// <summary>Miscellaneous programming utilities.</summary>
///
/// Contains various utility classes and functions used by glge.
///
/// \file util.h

#pragma once

#include <glge/common.h>

#include <functional>
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
	void print_nested_exception(const std::exception & e,
								unsigned int level = 0);

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
