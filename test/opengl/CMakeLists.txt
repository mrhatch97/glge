if(GLGE_HEADLESS_TESTS)
	message("-- glge OpenGL tests enabled (headless)")

	find_package(OpenGL REQUIRED COMPONENTS EGL)

else()
	find_package(glfw3 QUIET)

	if(NOT ${glfw3_FOUND})
		message("-- skipping glge OpenGL tests; OpenGL context requires headless (EGL) or GLFW")
		return()
	endif()

	message("-- glge OpenGL tests enabled (GLFW)")

endif()

find_package(GLEW REQUIRED)

macro(add_quick_test test_name)
	_add_quick_test(${ARGV})

	target_include_directories(test_${test_name}
		PRIVATE
			${PROJECT_SOURCE_DIR}/test/opengl/include)

	if(GLGE_HEADLESS_TESTS)
		target_compile_definitions(test_${test_name}
			PRIVATE
				GLGE_HEADLESS_TESTS
		)
	else()
		target_link_libraries(test_${test_name}
			PRIVATE
				glfw)
	endif()

endmacro()

add_quick_test(ogl_configure_env)
add_quick_test(ogl_load_model)
add_quick_test(ogl_load_cubemap)
add_quick_test(ogl_load_texture)
add_quick_test(ogl_load_shader)
add_quick_test(ogl_parameterize_shader)
add_quick_test(ogl_scene_traverse)
add_quick_test(ogl_scene_render)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/test/resources/textures)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/test/resources/cubemaps)

add_custom_command(TARGET test_ogl_load_texture POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/test/resources/textures/test.png ${PROJECT_BINARY_DIR}/test/resources/textures)

file(GLOB TEST_CUBEMAPS ${PROJECT_SOURCE_DIR}/test/resources/cubemaps/*.tga)

add_custom_command(TARGET test_ogl_load_cubemap POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${TEST_CUBEMAPS} ${PROJECT_BINARY_DIR}/test/resources/cubemaps)

if(WIN32)
	set(GLEW_DLL_NAME glew32$<$<CONFIG:Debug>:d>.dll)
	set(GLEW_DLL_LOCATION $<TARGET_FILE_DIR:GLEW::GLEW>/../bin/${GLEW_DLL_NAME})

	add_custom_command(TARGET test_ogl_load_model POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${GLEW_DLL_LOCATION} ${PROJECT_BINARY_DIR}/test/opengl
		COMMENT "Copying ${GLEW_DLL_LOCATION} to test directory")
endif()
