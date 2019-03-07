#pragma once

#include <glge/renderer/scene_graph/node.h>
#include <glge/renderer/scene_graph/scene_settings.h>

#include <memory>

namespace glge::renderer::scene_graph
{
	class Scene
	{
	private:
		Vector<SceneCamera *> cameras;
		uptr<Node> root;

	public:
		SceneSettings settings;

		Scene(std::unique_ptr<Node> && root);
		~Scene();

		Scene & operator=(Scene && other) = default;

		void draw() const;

		void view_resized(int width, int height);

		SceneCamera & get_camera();

		const SceneCamera & get_camera() const;

		Node & get_root();

		const Node & get_root() const;

		void add_camera(SceneCamera & cam);
	};
}
