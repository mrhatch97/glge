/// <summary>Mathematical functions and helper classes.</summary>
///
/// \file math.h

#pragma once

#include <glge/common.h>

#include <array>

namespace glge::math
{
	/// <summary>
	/// Class for incrementing and decrementing a value by a constant step.
	/// </summary>
	/// <typeparam name="T">
	/// Type of the underlying value of the stepper.
	/// </typeparam>
	template<typename T>
	class Stepper
	{
	private:
		T val;

	public:
		/// <summary>Amount to increase or decrease value by per step.</summary>
		const T step;

		/// <summary>
		/// Gets the current value of the Stepper.
		/// </summary>
		/// <returns>Current value of the stepper.</returns>
		T get() const { return val; }

		/// <summary>
		/// Implicitly gets the current value of the Stepper.
		/// </summary>
		/// <returns>Current value of the stepper.</returns>
		operator T() const { return val; }

		/// <summary>Increment the value of the Stepper by one step.</summary>
		/// <returns>New value of the Stepper.</returns>
		T operator++()
		{
			val += step;
			return val;
		}

		/// <summary>Increment the value of the Stepper by one step.</summary>
		/// <returns>Old value of the Stepper.</returns>
		T operator++(int)
		{
			T old = val;
			val += step;
			return old;
		}

		/// <summary>Decrement the value of the Stepper by one step.</summary>
		/// <returns>New value of the Stepper.</returns>
		T operator--()
		{
			val -= step;
			return val;
		}

		/// <summary>Decrement the value of the Stepper by one step.</summary>
		/// <returns>Old value of the Stepper.</returns>
		T operator--(int)
		{
			T old = val;
			val -= step;
			return old;
		}

		/// <summary>
		/// Constructs a new Stepper with the given start value and step.
		/// </summary>
		Stepper(const T & start, const T & step) : val(start), step(step) {}
	};

	/// <summary>
	/// Class for oscillating a value between a minimum and maximum value.
	/// </summary>
	/// <typeparam name="T">
	/// Type of the underlying value of the oscillator.
	/// </typeparam>
	template<typename T>
	class Oscillator
	{
	private:
		T val;
		bool increasing;

		void step(T delta)
		{
			if (!increasing)
			{
				delta = -delta;
			}

			if (val > max - delta || val < min - delta)
			{
				increasing = !increasing;
				step(-delta);
				return;
			}
			else
			{
				val += delta;
			}
		}

	public:
		/// <summary>Amount to increase or decrease value by per step.</summary>
		const T delta;
		/// <summary>Minimum value of the oscillator.</summary>
		const T min;
		/// <summary>Maximum value of the oscillator.</summary>
		const T max;

		/// <summary>
		/// Gets the current value of the Oscillator.
		/// </summary>
		/// <returns>Current value of the Oscillator.</returns>
		T get() const { return val; }

		/// <summary>
		/// Implicitly gets the current value of the Oscillator.
		/// </summary>
		/// <returns>Current value of the Oscillator.</returns>
		operator T() const { return val; }

		/// <summary>
		/// Increment the value of the Oscillator by one step.
		/// </summary>
		/// <returns>New value of the Oscillator.</returns>
		T operator++()
		{
			step(delta);
			return val;
		}

		/// <summary>
		/// Increment the value of the Oscillator by one step.
		/// </summary>
		/// <returns>Old value of the Oscillator.</returns>
		T operator++(T)
		{
			T old = val;
			step(delta);
			return old;
		}

		/// <summary>
		/// Decrement the value of the Oscillator by one step.
		/// </summary>
		/// <returns>New value of the Oscillator.</returns>
		T operator--()
		{
			step(-delta);
			return val;
		}

		/// <summary>
		/// Decrement the value of the Oscillator by one step.
		/// </summary>
		/// <returns>Old value of the Oscillator.</returns>
		T operator--(T)
		{
			T old = val;
			step(-delta);
			return old;
		}

		/// <summary>
		/// Constructs a new Oscillator with the given start value, step, min,
		/// and max.
		/// </summary>
		Oscillator(const T & start,
				   const T & delta,
				   const T & min,
				   const T & max) :
			increasing(true),
			val(start), delta(delta), min(min), max(max)
		{
			if (min > max)
			{
				throw std::logic_error("Oscillator min greater than max");
			}
			// TODO deal with overflow? unsure if possible
			if (std::abs(delta) >= std::abs(max - min))
			{
				throw std::logic_error("Oscillator delta is greater than the "
									   "distance between max and min");
			}
		}
	};

	/// <summary>
	/// Clamp a value to a range, wrapping between max and min as needed.
	/// </summary>
	/// <param name="val">Value to clamp.</param>
	/// <param name="min">Minimum allowed value.</param>
	/// <param name="max">Maximum allowed value.</param>
	/// <typeparam name="T">Type of value to clamp.</typeparam>
	template<typename T>
	T wrap_range(const T & val, const T & min, const T & max)
	{
		T cur_val = val;

		while (cur_val > max)
		{
			cur_val = min + (cur_val - max);
		}

		while (cur_val < min)
		{
			cur_val = max - (min - cur_val);
		}

		return cur_val;
	}

	/// <summary>Geometric sphere; a point and radius.</summary>
	struct Sphere
	{
		/// <summary>Radius of the Sphere.</summary>
		float radius;
		/// <summary>Origin point of the Sphere.</summary>
		vec3 origin;
	};

	/// <summary>
	/// Geometric 2D plane in 3D space, defined by a point and a
	/// normal vector.
	/// </summary>
	struct Plane
	{
	private:
		const float d;

	public:
		/// <summary>
		/// A point on the plane; defines its location in space.
		/// </summary>
		const vec3 point;
		/// <summary>
		/// A normal vector to the plane; defines its orientation in space.
		/// </summary>
		/// Defines the set of points on the plane, along with the point. The
		/// vector is considered to point towards the "inside" of the plane, for
		/// computations where that is relevant.
		const vec3 normal;

		/// <summary>
		/// Constructs a new Plane containing the given point and with the given
		/// normal vector.
		/// </summary>
		/// <param name="point">Point on the plane.</param>
		/// <param name="normal">Vector normal to the plane.</param>
		Plane(const vec3 & point, const vec3 & normal) :
			d(glm::dot(point, normal)), point(point), normal(normal)
		{}

		/// <summary>Compute the distance from the plane to a point.</summary>
		/// <param name="pt">Point to compute distance to.</param>
		/// <returns>
		/// Distance from the nearest point on the plane to the given point.
		/// Negative if the point is "outside" the plane.
		/// </returns>
		float distance_from(const vec3 & pt) const;
	};

	/// <summary>A geometric pyramidal frustum.</summary>
	/// A set of 6 planes encompassing a space in the shape of a clipped
	/// pyramid.
	struct Frustum
	{
		const Plane near,   ///< <summary>Near plane.</summary>
			far,            ///< <summary>Far plane.</summary>
			left,           ///< <summary>Left plane.</summary>
			right,          ///< <summary>Right plane.</summary>
			bottom,         ///< <summary>Bottom plane.</summary>
			top;            ///< <summary>Top plane.</summary>

		/// <summary>
		/// Constructs a Frustum with the given array of 6 Planes.
		/// </summary>
		/// <param name="planes">Array of 6 Planes comprising the
		/// Frustum.</param>
		Frustum(const std::array<Plane, 6> & planes);
	};

	/// <summary>
	/// Test whether a Sphere is inside a Plane.
	/// The normal vector of a plane points towards the "inside" direction.
	/// </summary>
	/// <returns>True if the sphere is inside the plane.</returns>
	bool contains(Plane plane, Sphere sphere);

	/// <summary>
	/// Test whether a Sphere is inside a Frustum.
	/// </summary>
	bool contains(Frustum frustum, Sphere sphere);

	/// <summary>
	/// Representation of a cubic Bezier curve.
	/// </summary>
	struct BezierCurve
	{
	private:
		const mat4 points;
		static const mat4 basis;

	public:
		/// <summary>
		/// Constructs a BezierCurve with the given 4 control points.
		/// </summary>
		BezierCurve(const vec3 & p0,
					const vec3 & p1,
					const vec3 & p2,
					const vec3 & p3) :
			points(vec4(p0, 1.0f),
				   vec4(p1, 1.0f),
				   vec4(p2, 1.0f),
				   vec4(p3, 1.0f))
		{}

		/// <summary>
		/// Compute the point on the curve for the given value of t.
		/// </summary>
		/// <param name="t">
		/// Control parameter; must be in the range [0, 1]. Describes the
		/// progress along the curve of the point.
		/// </param>
		/// <returns>Computed point.</returns>
		vec3 evaluate_at(const float t) const;

		/// <summary>
		/// Compute the tangent on the curve for the given value of t.
		/// </summary>
		/// <param name="t">
		/// Control parameter; must be in the range [0, 1]. Describes the
		/// progress along the curve of the point.
		/// </param>
		/// <returns>Computed tangent.</returns>
		vec3 velocity_at(const float t) const;
	};

	/// <summary>
	/// Handle on a Bezier path to enforce C1 continuity.
	/// </summary>
	struct BezierHandle
	{
		/// <summary>Interpolating point; point on the curve.</summary>
		vec3 interp_point;
		/// <summary>Control point; one side of the handle.</summary>
		vec3 control_point;

		/// <summary>Control point on the opposite side of the handle.</summary>
		/// <returns>
		/// Control point on the line between the interpolating point
		/// and concrete control point.
		/// </returns>
		vec3 opposite_control_point() const
		{
			return interp_point - (control_point - interp_point);
		}
	};

	/// <summary>
	/// A series of two or more BezierHandles defining a
	/// composite Bezier curve.
	/// </summary>
	struct BezierPath
	{
		/// <summary>
		/// Collection of control handles. Must be at least 2.
		/// </summary>
		std::vector<BezierHandle> handles;

		/// <summary>
		/// Compute the point on the composite curve for the given value of t.
		/// </summary>
		/// <param name="t">
		/// Control parameter; must be in the range [0, 1]. Describes the
		/// progress along the curve of the point.
		/// </param>
		/// <returns>Computed point.</returns>
		vec3 evaluate_at(float t) const;

		/// <summary>
		/// Compute the tangent on the composite curve for the given value of t.
		/// </summary>
		/// <param name="t">
		/// Control parameter; must be in the range [0, 1]. Describes the
		/// progress along the curve of the point.
		/// </param>
		/// <returns>Computed tangent.</returns>
		vec3 velocity_at(float t) const;

		/// <summary>
		/// Compute a set of points on the composite curve for the given
		/// values of t.
		/// </summary>
		/// <param name="ts">
		/// Control parameters; must be in the range [0, 1]. Each describes the
		/// progress along the curve of the point.
		/// </param>
		/// <returns>Computed points.</returns>
		vector<vec3> evaluate_at(const vector<float> & ts) const;

		/// <summary>
		/// Compute a set of equidistant points on the composite curve.
		/// </summary>
		/// <param name="samples_per_path">
		/// Number of sample points to compute.
		/// </param>
		/// <returns>Computed points.</returns>
		vector<vec3> sample(unsigned int samples_per_path) const;
	};

	/// <summary>Compare vectors by x-coordinate value.</summary>
	/// <returns>True if first vector is less than second.</returns>
	bool compare_by_x(vec3, vec3);
	/// <summary>Compare vectors by y-coordinate value.</summary>
	/// <returns>True if first vector is less than second.</returns>
	bool compare_by_y(vec3, vec3);
	/// <summary>Compare vectors by z-coordinate value.</summary>
	/// <returns>True if first vector is less than second.</returns>
	bool compare_by_z(vec3, vec3);

	/// <summary>Compare vectors by x-coordinate absolute value.</summary>
	/// <returns>True if first vector is less than second.</returns>
	bool compare_by_x_abs(vec3, vec3);
	/// <summary>Compare vectors by y-coordinate absolute value.</summary>
	/// <returns>True if first vector is less than second.</returns>
	bool compare_by_y_abs(vec3, vec3);
	/// <summary>Compare vectors by z-coordinate absolute value.</summary>
	/// <returns>True if first vector is less than second.</returns>
	bool compare_by_z_abs(vec3, vec3);

	/// <summary>Compare vectors by magnitude.</summary>
	/// <returns>True if first vector is greater than second.</returns>
	bool compare_by_magnitude(vec3, vec3);

	/// <summary>
	/// Compute a normalized vector from the center of a virtual ball
	/// to a point on the surface of the ball, using a point in a window.
	/// </summary>
	/// <param name="window_width">Width of the window.</param>
	/// <param name="window_height">Height of the window.</param>
	/// <param name="x">x-coordinate in window.</param>
	/// <param name="y">y-coordinate in window.</param>
	vec3
	trackball_point(float window_width, float window_height, float x, float y);

	/// <summary>
	/// Compute a rotation quaternion to rotate from one direction vector to
	/// another.
	/// </summary>
	/// <param name="start">Vector to rotate from.</param>
	/// <param name="target">Vector to rotate to.</param>
	quat rotation_between_vectors(vec3 start, vec3 target);
}   // namespace glge::math
