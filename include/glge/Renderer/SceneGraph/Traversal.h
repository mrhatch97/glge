#pragma once

#include <glge/Common.h>
#include <glge/Renderer/Renderer.h>
#include <glge/Renderer/SceneGraph/SceneSettings.h>

namespace eng::render::scene
{
	struct Node;
	struct Geometry;
	struct DynamicTransform;
	struct StaticTransform;
	struct SceneCamera;

	// TODO use CRTP to optimize choice of dispatcher?
	class BaseSceneDispatcher
	{
	protected:
		SceneSettings settings;

	public:
		virtual void dispatch(const Geometry & node) const = 0;
		virtual void dispatch(const DynamicTransform & node) const = 0;
		virtual void dispatch(const StaticTransform & node) const = 0;
		virtual void dispatch(const SceneCamera & node) const = 0;

		BaseSceneDispatcher(const SceneSettings & settings);
	};

	class RenderingSceneDispatcher : BaseSceneDispatcher
	{
	public:
		Renderer & renderer;
		mat4 cur_M;

		RenderingSceneDispatcher(const SceneSettings & settings, Renderer & renderer);

		void dispatch(const Geometry & node) const override;
		void dispatch(const DynamicTransform & node) const override;
		void dispatch(const StaticTransform & node) const override;
		void dispatch(const SceneCamera & node) const override;
	};
}