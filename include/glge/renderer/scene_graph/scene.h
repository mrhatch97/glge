/// <summary>Classes for represending a 3D scene as a scene graph.</summary>
///
/// \file scene.h
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

  /// <summary>
  /// Object allowing manipulation of nodes within a Scene
  /// </summary>
  class NodeHandle
  {
    protected:
      /// <summary>Parent of the pointed-to node.</summary>
      observer_ptr<Node> parent;
      /// <summary>The node being pointed to.</summary>
      Node & node;
      /// <summary>The Scene this node belongs to.</summary>
      Scene & scene;
    public:
      /// <summary>
      /// Constructs a new NodeHandle with the given parent, 
      /// node, and scene.
      /// </summary>
      NodeHandle(observer_ptr<Node> parent, Node & node, Scene & scene);

      /// <summary>
      /// Add geometry to the scene under this node.
      /// </summary>
      /// <param name="renderable">
      /// Renderable object to draw at this node.
      /// </param>
      /// <param name="shader">
      /// Shader to use to draw this node.
      /// </param>
      /// <returns>NodeHandle of the created node.</returns>
      NodeHandle add_geometry(
          primitive::Renderable & renderable, 
          primitive::ShaderInstanceBase & shader
      );

      /// <summary>
      /// Add a transform to the scene under this node.
      /// </summary>
      /// <param name="placement">
      /// Reference to the Placement used to control this transform. Not
      /// copied.
      /// </param>
      /// <returns>NodeHandle of the created node.</returns>
      NodeHandle add_transform(const util::Placement & placement);

      /// <summary>
      /// Add a camera to the scene under this node.
      /// </summary>
      /// <param name="intrinsics">
      /// Intrinsic properties of the camera to add.
      /// </param>
      /// <returns>NodeHandle of the created node.</returns>
      CameraHandle add_camera(const CameraIntrinsics & intrinsics);
  };

  /// <summary>
  /// Object allowing manipulation of camera nodes within a Scene
  /// </summary>
  class CameraHandle : public NodeHandle
  {
    public:
      /// <summary>
      /// Constructs a new CameraHandle with the given parent, 
      /// node, and scene.
      /// </summary>
      CameraHandle(
          observer_ptr<Node> parent, 
          SceneCamera & node, 
          Scene & scene);

      /// <summary>
      /// Sets this camera node as the active camera in the scene.
      /// </summary>
      void activate();
  };

  /// <summary>
  /// Scene graph representing a renderable 3D scene.
  /// </summary>
	class Scene
	{
	private:
		unique_ptr<Node> root;

	public:
    /// <summary>
    /// Configuration object for this Scene.
    /// </summary>
		SceneSettings settings;

    /// <summary>
    /// Constructs a new, empty Scene.
    /// </summary>
		Scene();

    /// <summary>
    /// Traverses the scene graph and creates a Renderer
    /// whose state reflects the graph at the time of
    /// traversal.
    /// </summary>
    /// <returns>
    /// Renderer to render the 3D scene described by this object.
    /// </returns>
		Renderer prepare_renderer() const;

    /// <summary>
    /// Get a NodeHandle to the root of this Scene to allow
    /// adding new nodes to the graph.
    /// </summary>
    /// <returns>NodeHandle for the root node of the Scene.</summary>
    NodeHandle get_root_handle();

		~Scene();
	};
}
