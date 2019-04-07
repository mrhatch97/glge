target_sources(glge
	PRIVATE
		renderer.cpp
		render_settings.cpp
		camera.cpp
		engine.cpp
		primitives/shader_program.cpp
		primitives/primitive_data.cpp
    scene_graph/scene_settings.cpp
		scene_graph/scene.cpp
		scene_graph/traversal.cpp
)

target_include_directories(glge
	PRIVATE
		scene_graph)

if(GLGE_DRIVER STREQUAL "OPENGL")
	add_subdirectory(opengl)
endif()
