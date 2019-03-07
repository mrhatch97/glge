target_sources(glge
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/renderer.cpp
		${CMAKE_CURRENT_LIST_DIR}/render_settings.cpp
		${CMAKE_CURRENT_LIST_DIR}/camera.cpp
		${CMAKE_CURRENT_LIST_DIR}/engine.cpp
		${CMAKE_CURRENT_LIST_DIR}/primitives/cubemap.cpp
		${CMAKE_CURRENT_LIST_DIR}/primitives/lines.cpp
		${CMAKE_CURRENT_LIST_DIR}/primitives/shader_program.cpp
		${CMAKE_CURRENT_LIST_DIR}/primitives/texture.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/resources/resource_manager.cpp
		${CMAKE_CURRENT_LIST_DIR}/scene_graph/dynamic_transform.cpp
		${CMAKE_CURRENT_LIST_DIR}/scene_graph/node.cpp
		${CMAKE_CURRENT_LIST_DIR}/scene_graph/scene.cpp
		${CMAKE_CURRENT_LIST_DIR}/scene_graph/scene_camera.cpp
		${CMAKE_CURRENT_LIST_DIR}/scene_graph/static_transform.cpp
		${CMAKE_CURRENT_LIST_DIR}/scene_graph/traversal.cpp
)

target_include_directories(glge
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/resources
)

add_subdirectory(primitives/opengl)
