#include "glge/util/math.h"

#include <glge/util/util.h>

#include <algorithm>

#include <cmath>
#include <numeric>

namespace glge::math
{
	float Plane::distance_from(const vec3 & pt) const
	{
		return glm::dot(pt, normal) - d;
	}

	Frustum::Frustum(const std::array<Plane, 6> & planes) : near(planes[0]), far(planes[1]),
		left(planes[2]), right(planes[3]), bottom(planes[4]), top(planes[5]) { }

	bool contains(Frustum frustum, Sphere sphere)
	{
		return contains(frustum.near, sphere) && contains(frustum.far, sphere) &&
			contains(frustum.left, sphere) && contains(frustum.right, sphere) &&
			contains(frustum.bottom, sphere) && contains(frustum.top, sphere);
	}

	bool contains(Plane plane, Sphere sphere)
	{
		return plane.distance_from(sphere.origin) > (-sphere.radius);
	}

	// Geometric basis matrix for a Cubic bezier curve - universally constant
	const mat4 BezierCurve::basis = mat4(vec4{ -1, 3, -3, 1 }, vec4{ 3, -6, 3, 0 }, vec4{ -3, 3, 0, 0 }, vec4{ 1, 0, 0, 0 });

	vec3 BezierCurve::evaluateAt(const float t) const
	{
		if (t > 1.0f || t < 0.0f)
		{
			throw std::domain_error(EXC_MSG("A bezier curve cannot be evaluated at a range outside [0, 1]"));
		}

		vec4 T(t * t * t, t * t, t, 1);

		return points * basis * T;
	}

	vec3 BezierCurve::velocityAt(const float t) const
	{
		if (t > 1.0f || t < 0.0f)
		{
			throw std::domain_error(EXC_MSG("A bezier curve cannot be evaluated at a range outside [0, 1]"));
		}

		vec4 T(3 * t * t, 2 * t, 1, 0);

		return points * basis * T;
	}

	vec3 BezierPath::evaluateAt(const float t) const
	{
		if (handles.size() < 1)
		{
			throw std::logic_error(EXC_MSG("A bezier path must have at least 1 handle to define a full curve"));
		}

		if (t > 1.0f || t < 0.0f)
		{
			throw std::domain_error(EXC_MSG("A bezier path cannot be evaluated at a range outside [0, 1]"));
		}

		size_t num_curves = static_cast<size_t>(handles.size());

		auto offset_t = t * num_curves;
		auto offset_t_floor = glm::floor(offset_t);

		int first_handle_idx = static_cast<int>(offset_t_floor);
		int second_handle_idx = static_cast<int>(offset_t_floor + 1.0f);

#if _DEBUG
		if (first_handle_idx == second_handle_idx)
		{
			throw std::logic_error(EXC_MSG("Handles for path curve were identical"));
		}
#endif
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
		vec3 p1 = first_handle->otherControlPoint();
		vec3 p2 = second_handle->control_point;
		vec3 p3 = second_handle->interp_point;

		return BezierCurve(p0, p1, p2, p3).evaluateAt(offset_t - offset_t_floor);
	}

	vec3 BezierPath::velocityAt(const float t) const
	{
		if (handles.size() < 1)
		{
			throw std::logic_error(EXC_MSG("A bezier path must have at least 1 handle to define a full curve"));
		}

		if (t > 1.0f || t < 0.0f)
		{
			throw std::domain_error(EXC_MSG("A bezier path cannot be evaluated at a range outside [0, 1]"));
		}

		size_t num_curves = static_cast<size_t>(handles.size());

		auto offset_t = t * num_curves;
		auto offset_t_floor = glm::floor(offset_t);

		int first_handle_idx = static_cast<int>(offset_t_floor);
		int second_handle_idx = static_cast<int>(offset_t_floor + 1.0f);

#if _DEBUG
		if (first_handle_idx == second_handle_idx)
		{
			throw std::logic_error(EXC_MSG("Handles for path curve were identical"));
		}
#endif
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
		vec3 p1 = first_handle->otherControlPoint();
		vec3 p2 = second_handle->control_point;
		vec3 p3 = second_handle->interp_point;

		return BezierCurve(p0, p1, p2, p3).velocityAt(offset_t - offset_t_floor);
	}

	vector<vec3> BezierPath::evaluateAt(const vector<float> & ts) const
	{
		vector<vec3> result(ts.size());
		std::transform(ts.cbegin(), ts.cend(), result.begin(), 
        [&](const float t) { return this->evaluateAt(t); });
		return result;
	}

	vector<vec3> BezierPath::sample(const unsigned int samples_per_path) const
	{
		const size_t num_samples = samples_per_path * handles.size();
		vector<float> ts(num_samples);
		const float step = 1.0f / (num_samples);
		std::iota(ts.begin(), ts.end(), math::Stepper(0.0f, step));

		return evaluateAt(ts);
	}

	bool compare_by_x(vec3 v1, vec3 v2)
	{
		return v1.x < v2.x;
	}

	bool compare_by_y(vec3 v1, vec3 v2)
	{
		return v1.y < v2.y;
	}

	bool compare_by_z(vec3 v1, vec3 v2)
	{
		return v1.z < v2.z;
	}

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
		return glm::length(v1) < glm::length(v2);
	}

	vec3 trackball_point(float window_width, float window_height, 
      float x, float y)
	{
    // Convert (x, y) to [-1, 1] space
		glm::vec3 v(
        (2.0f * x - window_width) / window_width,
        (window_height - 2.0f * y) / window_height,
        0.0f);

    // Distance from ball center must be at least 1
		float d = std::max(glm::length(v), 1.0f);

		v.z = glm::sqrt(1.001f - d * d);

		return glm::normalize(v);
	}

	quat rotation_between_vectors(vec3 start, vec3 target)
	{
		start = normalize(start);
		target = normalize(target);

		float cos_theta = glm::dot(start, target);
		vec3 rotation_axis;

		if (cos_theta < std::nextafter(-1.0f, 0.0f))
		{
			// special case when vectors in opposite directions:
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			rotation_axis = cross(vec3(0.0f, 0.0f, 1.0f), start);

			if (glm::length2(rotation_axis) < std::nextafter(0.0f, 1.0f)) 
			{
        // parallel - try again
				rotation_axis = cross(vec3(1.0f, 0.0f, 0.0f), start);
			}

			rotation_axis = glm::normalize(rotation_axis);
			return glm::angleAxis(glm::radians(180.0f), rotation_axis);
		}

		rotation_axis = cross(start, target);

		float s = sqrt((1 + cos_theta) * 2);
		float invs = 1 / s;

		return quat(
			s * 0.5f,
			rotation_axis.x * invs,
			rotation_axis.y * invs,
			rotation_axis.z * invs
		);
	}
}
