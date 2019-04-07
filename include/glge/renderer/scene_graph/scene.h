#pragma once

#include <glge/common.h>
#include <glge/renderer/scene_graph/scene_settings.h>
#include <glge/renderer/renderer.h>

namespace glge::renderer::scene_graph
{
	struct Node;
  struct SceneCamera;
  class Scene;
  class CameraHandle;

  class NodeHandle
  {
    protected:
      observer_ptr<Node> parent;
      Node & node;
      Scene & scene;
    public:
      NodeHandle(observer_ptr<Node> parent, Node & node, Scene & scene);

      NodeHandle add_geometry(
          primitive::Renderable & renderable, 
          primitive::ShaderInstanceBase & shader
      );
      NodeHandle add_transform(const util::Placement & placement);
      CameraHandle add_camera(const CameraIntrinsics & intrinsics);
  };

  class CameraHandle : public NodeHandle
  {
    public:
      CameraHandle(observer_ptr<Node> parent, SceneCamera & node, Scene & scene);

      void activate();
  };

	class Scene
	{
	private:
		unique_ptr<Node> root;

	public:
		SceneSettings settings;

		Scene();
		~Scene();

		Scene & operator=(Scene && other) = default;

		Renderer prepare_renderer() const;

    NodeHandle get_root_handle();
	};
}
