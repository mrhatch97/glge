/// \brief Mathematical functions and helper classes.
///
/// Contains mathematical functions and helper classes used by glge.
///
/// \file math.h

#pragma once

#include <glge/common.h>

#include <array>

namespace glge::math
{
	template<typename T>
	struct Stepper
	{
	private:
		T val;
	public:
		const T step;

		T get() const { return val; }

		operator T() const { return val; }

		T operator++() { val += step; return val; }

		T operator++(int) { T old = val; val += step; return old; }

		T operator--() { val -= step; return val; }

		T operator--(int) { T old = val; val -= step; return old; }

		Stepper(const T & start, const T & step) :
			val(start), step(step)
		{ }
	};

	template<typename T>
	struct Oscillator
	{
	private:
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
		T val;
		const T delta;
		const T min;
		const T max;

		T get() const { return val; }

		operator T() const { return val; }

		T operator++()
		{
			step(delta);
			return val;
		}

		T operator++(T)
		{
			T old = val;
			step(delta);
			return old;
		}

		T operator--()
		{
			step(-delta);
			return val;
		}

		T operator--(T)
		{
			T old = val;
			step(-delta);
			return old;
		}

		Oscillator(const T & start, const T & delta, const T & min, const T & max)
			: increasing(true), val(start), delta(delta), min(min), max(max)
		{
			if (min > max)
			{
				throw std::logic_error("Oscillator min greater than max");
			}
			// TODO deal with overflow? unsure if possible
			if (std::abs(delta) >= std::abs(max - min))
			{
				throw std::logic_error("Oscillator delta is greater than the distance between max and min");
			}
		}
	};

	/// <summary>Clamp a value to a range, wrapping between max and min as needed.</summary>
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

	/// <summary>Geometric 2D plane in 3D space, defined by a point and a
	/// normal vector.</summary>
	struct Plane
	{
	private:
		const float d;
	public:
		/// <summary>A point on the plane; defines its location in space.</summary>
		const vec3 point;
		/// <summary>A normal vector to the plane; defines its orientation in space.</summary>
		/// Defines the set of points on the plane, along with the point. The vector
		/// is considered to point towards the "inside" of the plane, for computations
		/// where that is relevant.
		const vec3 normal;

		Plane(const vec3 & point, const vec3 & normal) : d(glm::dot(point, normal)), point(point), normal(normal)
		{
		}

		/// <summary>Compute the distance from the plane to a point.</summary>
		/// <param name="pt">Point to compute distance to.</param>
		/// <returns>Distance from the nearest point on the plane to the given point.
		/// Negative if the point is "outside" the plane.</returns>
		float distance_from(const vec3 & pt) const;
	};

	/// <summary>A geometric pyramidal frustum.</summary>
	/// A set of 6 planes encompassing a space in the shape of a clipped pyramid.
	struct Frustum
	{
		const Plane near,   ///< <summary>Near plane.</summary>
								far,    ///< <summary>Far plane.</summary>
								left,   ///< <summary>Left plane.</summary>
								right,  ///< <summary>Right plane.</summary>
								bottom, ///< <summary>Bottom plane.</summary>
								top;    ///< <summary>Top plane.</summary>

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

	struct BezierCurve
	{
	private:
		const mat4 points;
		static const mat4 basis;
	public:
		BezierCurve(const vec3 & p0, const vec3 & p1, const vec3 & p2, const vec3 & p3) :
			points(vec4(p0, 1.0f), vec4(p1, 1.0f), vec4(p2, 1.0f), vec4(p3, 1.0f))
		{ }

		vec3 evaluateAt(const float t) const;
		vec3 velocityAt(const float t) const;
	};

	struct BezierHandle
	{
		vec3 interp_point;
		vec3 control_point;
		vec3 otherControlPoint() const
		{
			return interp_point - (control_point - interp_point);
		}
	};

	struct BezierPath
	{
		std::vector<BezierHandle> handles;

		vec3 evaluateAt(float t) const;
		vec3 velocityAt(float t) const;
		vector<vec3> evaluateAt(const vector<float> & ts) const;
		vector<vec3> sample(unsigned int samples_per_path) const;
	};

	bool compareByX(vec3, vec3);
	bool compareByY(vec3, vec3);
	bool compareByZ(vec3, vec3);

	bool compareByAbsX(vec3, vec3);
	bool compareByAbsY(vec3, vec3);
	bool compareByAbsZ(vec3, vec3);

	bool compareByMagnitude(vec3, vec3);

	vec3 trackballPoint(double windowX, double windowY, double x, double y);

	mat4 homogenousMatrix(float v);

	quat rotation_between_vectors(vec3 start, vec3 target);
	quat orient_object(vec3 default_direction, vec3 direction, vec3 default_up, vec3 up_dir);
}
