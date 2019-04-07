#include "glge/renderer/scene_graph/scene.h"

#include "node.h"
#include "geometry.h"
#include "transform.h"
#include "scene_camera.h"

namespace glge::renderer::scene_graph
{
  NodeHandle::NodeHandle(observer_ptr<Node> parent, Node & node, 
      Scene & scene) :
    parent(parent), node(node), scene(scene)
  { }

  NodeHandle NodeHandle::add_geometry(primitive::Renderable & renderable, 
      primitive::ShaderInstanceBase & shader)
  {
    auto & new_node_ptr = node.children.emplace_front(
        std::make_unique<Geometry>(renderable, shader));

    return NodeHandle(&node, *new_node_ptr, scene);
  }

  NodeHandle NodeHandle::add_transform(const util::Placement & placement)
  {
    auto & new_node_ptr = node.children.emplace_front(
        std::make_unique<SceneTransform>(placement));

    return NodeHandle(&node, *new_node_ptr, scene);
  }

  CameraHandle NodeHandle::add_camera(const CameraIntrinsics & intrinsics)
  {
    auto & new_node_ptr = node.children.emplace_front(
        std::make_unique<SceneCamera>(intrinsics));

    return CameraHandle(&node, static_cast<SceneCamera &>(*new_node_ptr), 
        scene);
  }

  CameraHandle::CameraHandle(observer_ptr<Node> parent, SceneCamera & node, 
      Scene & scene) :
    NodeHandle(parent, static_cast<Node &>(node), scene)
  { }

  void CameraHandle::activate()
  {
    static_cast<SceneCamera &>(node).active = true;
  }

	Scene::Scene() :
		root(std::make_unique<Node>()),
		settings(nullptr, false, false)
	{ }

  NodeHandle Scene::get_root_handle()
  {
    return NodeHandle(nullptr, *root, *this); 
  }

	Scene::~Scene() = default;
}
