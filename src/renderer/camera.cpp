#include "glge/renderer/camera.h"

namespace glge::renderer
{
	static constexpr float plane_height(math::Radians v_fov, float distance)
	{
		return 2 * glm::tan(static_cast<float>(v_fov) / 2.0f) * distance;
	}

	vec2 CameraIntrinsics::near_dimensions() const
	{
		auto height = plane_height(v_fov, near_distance);

		return vec2(height * aspect_ratio, height);
	}

	vec2 CameraIntrinsics::far_dimensions() const
	{
		auto height = plane_height(v_fov, far_distance);

		return vec2(height * aspect_ratio, height);
	}

	mat4 CameraIntrinsics::get_P() const
	{
		return glm::perspective(static_cast<float>(v_fov), aspect_ratio,
								near_distance, far_distance);
	}

	Camera::Camera(const CameraIntrinsics & intrinsics,
				   const util::Placement & placement) :
		intrinsics(intrinsics),
		placement(placement)
	{}

	mat4 Camera::get_V() const
	{
		mat4 cam2world = placement.transform;

		mat4 world_to_cam_rot = {
			{cam2world[0][0], cam2world[1][0], cam2world[2][0], 0},
			{cam2world[0][1], cam2world[1][1], cam2world[2][1], 0},
			{cam2world[0][2], cam2world[1][2], cam2world[2][2], 0},
			{0, 0, 0, 1}};

		mat4 world_to_cam_trans = {
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{-cam2world[3][0], -cam2world[3][1], -cam2world[3][2], 1}};

		mat4 world2cam = world_to_cam_rot * world_to_cam_trans;

		return world2cam;
	}

	math::Frustum Camera::get_view_frustum() const
	{
		auto near_dimensions = intrinsics.near_dimensions();
		auto right_vec = placement.get_right_direction();
		auto camera_vec = placement.get_forward_direction();
		auto up_vec = placement.get_up_direction();
		auto cam_pos = placement.get_position();

		auto near_center = cam_pos + camera_vec * intrinsics.near_distance;
		auto far_center = cam_pos + camera_vec * intrinsics.far_distance;

		math::Plane near(near_center, camera_vec);
		math::Plane far(far_center, -camera_vec);

		auto right_plane_normal = glm::normalize(
			(near_center + right_vec * near_dimensions.x / 2.0f) - cam_pos);
		auto left_plane_normal = glm::normalize(
			(near_center - right_vec * near_dimensions.x / 2.0f) - cam_pos);
		auto top_plane_normal = glm::normalize(
			(near_center + up_vec * near_dimensions.y / 2.0f) - cam_pos);
		auto bottom_plane_normal = glm::normalize(
			(near_center - up_vec * near_dimensions.y / 2.0f) - cam_pos);

		math::Plane right(cam_pos, glm::cross(up_vec, right_plane_normal));
		math::Plane left(cam_pos, -glm::cross(up_vec, left_plane_normal));
		math::Plane top(cam_pos, -glm::cross(right_vec, top_plane_normal));
		math::Plane bottom(cam_pos, glm::cross(right_vec, bottom_plane_normal));

		return std::array<math::Plane, 6>{near, far, right, left, top, bottom};
	}
}   // namespace glge::renderer
