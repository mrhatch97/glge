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

	struct Sphere
	{
		float radius;
		vec3 origin;
	};

	struct Plane
	{
	private:
		const float d;
	public:
		const vec3 point;
		const vec3 normal;

		Plane(const vec3 & point, const vec3 & normal) : d(glm::dot(point, normal)), point(point), normal(normal)
		{
		}

		float distance_from(const vec3 & pt) const;
	};

	struct Frustum
	{
		const Plane near, far, left, right, bottom, top;

		Frustum(const std::array<Plane, 6> & planes);
	};

	/// <summary>
	/// Test whether a <c>Sphere</c> is inside a <c>Plane</c>.
	/// The normal vector of a plane points towards the "inside" direction.
	/// </summary>
	/// <returns>True if the sphere is inside the plane.</returns>
	bool contains(Plane plane, Sphere sphere);

	/// <summary>
	/// Test whether a <c>Sphere</c> is inside a <c>Frustum</c>.
	/// </summary>
	/// <seealso cref="contains(Plane, Sphere)"/>
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
		Vector<vec3> evaluateAt(const Vector<float> & ts) const;
		Vector<vec3> sample(unsigned int samples_per_path) const;
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