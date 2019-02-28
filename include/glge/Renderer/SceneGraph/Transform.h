#pragma once

#include <glge/Renderer/SceneGraph/Node.h>

namespace eng::render::scene
{
	template<typename DerivedT>
	struct Transform_CRTP : public Node
	{
	private:
		DerivedT & crtp_base()
		{
			return static_cast<DerivedT &>(*this);
		}

		const DerivedT & crtp_base() const
		{
			return static_cast<const DerivedT &>(*this);
		}

	public:

		Transform_CRTP() = default;

		mat4 get_transform() const
		{
			return crtp_base().get_transform();
		}

		~Transform_CRTP() = default;
	};

	struct StaticTransform : public Transform_CRTP<StaticTransform>
	{
	private:
		const mat4 transform;
	public:

		StaticTransform(const mat4 & transform);
		StaticTransform(const quat & transform);
		StaticTransform(const StaticTransform & other);

		mat4 get_transform() const { return transform; }

		virtual void accept(const BaseSceneDispatcher& dispatcher) const;
	};

	struct DynamicTransform : public Transform_CRTP<DynamicTransform>
	{
	private:
		const util::Placement & placement;

	public:
		DynamicTransform(const util::Placement & placement);

		mat4 get_transform() const { return placement.get_transform(); }

		virtual void accept(const BaseSceneDispatcher& dispatcher) const;
	};
}