#include "glge/renderer/scene_graph/scene.h"

#include <glge/renderer/renderer.h>
#include <glge/util/util.h>

#include "base_dispatcher.h"
#include "geometry.h"
#include "scene_camera.h"
#include "transform.h"

#include <stack>
#include <tuple>

namespace glge::renderer::scene_graph
{
	class RenderingSceneDispatcher : public BaseDispatcher
	{
		Renderer & renderer;

	public:
		explicit RenderingSceneDispatcher(Renderer & renderer) :
			renderer(renderer)
		{}

		mat4 dispatch(const Node &, mat4 cur_M) const override { return cur_M; }

		mat4 dispatch(const Geometry & node, mat4 cur_M) const override
		{
			renderer.enqueue(node.renderable, node.shader, cur_M);
			return cur_M;
		}

		mat4 dispatch(const SceneTransform & transform,
					  mat4 cur_M) const override
		{
			return transform.placement.transform * cur_M;
		}

		mat4 dispatch(const SceneCamera & scene_camera,
					  mat4 cur_M) const override
		{
			if (scene_camera.active)
			{
				renderer.settings.camera = scene_camera.get_camera(cur_M);
			}
			return cur_M;
		}
	};

	Renderer Scene::prepare_renderer() const
	{
		using StateTuple = std::tuple<observer_ptr<const Node>, mat4>;

		std::stack<StateTuple> nodes;

		nodes.emplace(root.get(), mat4(1.0f));

		Renderer renderer;
		RenderingSceneDispatcher dispatcher(renderer);

		while (!nodes.empty())
		{
			StateTuple state = nodes.top();
			nodes.pop();

			auto [node_ptr, cur_M] = state;

			mat4 new_M = node_ptr->accept(dispatcher, cur_M);

			std::for_each(node_ptr->children.cbegin(),
						  node_ptr->children.cend(),
						  [&](const unique_ptr<Node> & node) {
							  nodes.emplace(node.get(), new_M);
						  });
		}

		if (!renderer.settings.camera)
		{
			throw std::logic_error(
				"Tried to render scene without an active camera");
		}

		return renderer;
	}
}   // namespace glge::renderer::scene_graph
