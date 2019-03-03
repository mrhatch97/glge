#pragma once

#include <glge/Common.h>
#include <glge/Util/Motion.h>
#include <glge/Util/math.h>
#include <glge/Renderer/Primitives/ShaderProgram.h>
#include <glge/Renderer/SceneGraph/SceneSettings.h>

#include <forward_list>

namespace eng::render::scene
{
	class BaseSceneDispatcher;

	struct Node
	{
	public:
		Node();

		std::forward_list<uptr<Node>> children;

		virtual ~Node();

		virtual void accept(const BaseSceneDispatcher & dispatcher) const = 0;
	};
}