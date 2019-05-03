#include "glge/util/math.h"

#include <glge/util/util.h>

#include <algorithm>
#include <cmath>
#include <numeric>

namespace glge::math
{
	float Plane::distance_from(vec3 pt) const
	{
		return glm::dot(pt, normal) - d;
	}

	bool Plane::operator==(Plane other) const
	{
		return other.normal == normal && contains(*this, other.point);
	}

	Frustum::Frustum(const std::array<Plane, 6> & planes) :
		near(planes[0]), far(planes[1]), left(planes[2]), right(planes[3]),
		bottom(planes[4]), top(planes[5])
	{}

	bool contains(Plane plane, vec3 point)
	{
		// A point that lies on the plane will be perpendicular wrt the plane
		// normal
		return glm::dot(point - plane.point, plane.normal) == 0.0f;
	}

	bool contains(Plane plane, Sphere sphere)
	{
		return plane.distance_from(sphere.origin) > (-sphere.radius);
	}

	bool contains(const Frustum & frustum, Sphere sphere)
	{
		return contains(frustum.near, sphere) &&
			   contains(frustum.far, sphere) &&
			   contains(frustum.left, sphere) &&
			   contains(frustum.right, sphere) &&
			   contains(frustum.bottom, sphere) &&
			   contains(frustum.top, sphere);
	}

	// Geometric basis matrix for a Cubic bezier curve - universally constant
	const mat4 BezierCurve::basis = mat4(vec4{-1, 3, -3, 1},
										 vec4{3, -6, 3, 0},
										 vec4{-3, 3, 0, 0},
										 vec4{1, 0, 0, 0});

	BezierCurve::BezierCurve(const vec3 & p0,
							 const vec3 & p1,
							 const vec3 & p2,
							 const vec3 & p3) :
		points(vec4(p0, 1.0f), vec4(p1, 1.0f), vec4(p2, 1.0f), vec4(p3, 1.0f))
	{}

	static bool verify_domain(float value, float min, float max)
	{
		return value >= min && value <= max;
	}

	vec3 BezierCurve::evaluate_at(float t,
								  const CurvePolynomial & polynomial) const
	{
		if (!verify_domain(t, min_t, max_t))
		{
			throw std::domain_error(
				EXC_MSG("A bezier curve cannot be evaluated at a range outside "
						"[0, 1]"));
		}

		auto get_T_term = [t](const PolynomialTerm & term) {
			return term.coefficient * std::pow(t, term.power);
		};

		vec4 T(get_T_term(polynomial[0]), get_T_term(polynomial[1]),
			   get_T_term(polynomial[2]), get_T_term(polynomial[3]));

		return points * basis * T;
	}

	vec3 BezierPath::evaluate_at(
		float t,
		const BezierCurve::CurvePolynomial & polynomial) const
	{
		if (handles.size() < 1)
		{
			throw std::logic_error(EXC_MSG("A bezier path must have at least 1"
										   "handle to define a full curve"));
		}

		if (!verify_domain(t, BezierCurve::min_t, BezierCurve::max_t))
		{
			throw std::domain_error(
				EXC_MSG("A bezier path cannot be evaluated at a"
						" range outside [0, 1]"));
		}

		size_t num_curves = handles.size();

		auto offset_t = t * num_curves;
		float offset_t_floor = glm::floor(offset_t);

		size_t first_handle_idx = static_cast<size_t>(offset_t_floor);
		size_t second_handle_idx = static_cast<size_t>(offset_t_floor + 1.0f);

		if constexpr (debug)
		{
			if (first_handle_idx == second_handle_idx)
			{
				throw std::logic_error(
					EXC_MSG("Handles for path curve were identical"));
			}
		}

		const math::BezierHandle * first_handle;
		const math::BezierHandle * second_handle;

		if (second_handle_idx == handles.size())
		{
			first_handle = &handles.at(first_handle_idx);
			second_handle = &handles.at(0);
		}
		else
		{
			first_handle = &handles.at(first_handle_idx);
			second_handle = &handles.at(second_handle_idx);
		}

		vec3 p0 = first_handle->interp_point;
		vec3 p1 = first_handle->opposite_control_point();
		vec3 p2 = second_handle->control_point;
		vec3 p3 = second_handle->interp_point;

		return BezierCurve(p0, p1, p2, p3)
			.evaluate_at(offset_t - offset_t_floor, polynomial);
	}

	vector<vec3> BezierPath::evaluate_at(
		const vector<float> & ts,
		const BezierCurve::CurvePolynomial & polynomial) const
	{
		vector<vec3> result(ts.size());
		std::transform(
			ts.cbegin(), ts.cend(), result.begin(),
			[&](const float t) { return this->evaluate_at(t, polynomial); });
		return result;
	}

	vector<vec3>
	BezierPath::sample(unsigned int samples_per_path,
					   const BezierCurve::CurvePolynomial & polynomial) const
	{
		const size_t num_samples = samples_per_path * handles.size();
		vector<float> ts(num_samples);
		const float step = 1.0f / (num_samples);
		std::iota(ts.begin(), ts.end(), math::Stepper(0.0f, step));

		return evaluate_at(ts, polynomial);
	}

	bool compare_by_x(vec3 v1, vec3 v2) { return v1.x < v2.x; }

	bool compare_by_y(vec3 v1, vec3 v2) { return v1.y < v2.y; }

	bool compare_by_z(vec3 v1, vec3 v2) { return v1.z < v2.z; }

	bool compare_by_x_abs(vec3 v1, vec3 v2)
	{
		return glm::abs(v1.x) < glm::abs(v2.x);
	}

	bool compare_by_y_abs(vec3 v1, vec3 v2)
	{
		return glm::abs(v1.y) < glm::abs(v2.y);
	}

	bool compare_by_z_abs(vec3 v1, vec3 v2)
	{
		return glm::abs(v1.z) < glm::abs(v2.z);
	}

	bool compare_by_magnitude(vec3 v1, vec3 v2)
	{
		// Cheaper to compare length squared (no square root needed)
		return glm::length2(v1) < glm::length2(v2);
	}

	vec3
	trackball_point(float window_width, float window_height, float x, float y)
	{
		// Convert (x, y) to [-1, 1] space
		glm::vec3 v((2.0f * x - window_width) / window_width,
					(window_height - 2.0f * y) / window_height, 0.0f);

		// Distance from ball center must be at least 1
		float d = std::max(glm::length(v), 1.0f);

		// Remaining component is in z-direction
		v.z = glm::sqrt(std::nextafter(1.0f, 2.0f) - d * d);

		return glm::normalize(v);
	}
}   // namespace glge::math
