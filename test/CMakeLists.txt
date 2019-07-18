message("-- glge tests enabled")

# Target to test that depends on the build of the test executables
add_custom_target(build_tests)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS build_tests)

add_library(glge_test INTERFACE)

target_include_directories(glge_test
	INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/include
		${PROJECT_SOURCE_DIR}/src/include
)

macro(add_quick_test test_name)
	add_executable(test_${test_name}
		test_${test_name}.cpp)

	target_link_libraries(test_${test_name}
		PRIVATE
			glge
			glge_test)

	add_test(NAME ${test_name}
		COMMAND test_${test_name}
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/test)

	add_dependencies(build_tests test_${test_name})
endmacro()

add_quick_test(safe_cast)
add_quick_test(returns_void)
add_quick_test(contains)
add_quick_test(stepper)
add_quick_test(circular)
add_quick_test(oscillator)
add_quick_test(bezier_curve)
add_quick_test(compare)
add_quick_test(misc_math)
add_quick_test(heightmap_util)
add_quick_test(unique_handle)
add_quick_test(events)
add_quick_test(input)
add_quick_test(parse_model)
add_quick_test(parse_model_big)
add_quick_test(model_to_EBO)
add_quick_test(motion)
add_quick_test(camera)
add_quick_test(heightmap_gen)
add_quick_test(l_system)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/test/resources/models)

add_custom_command(TARGET test_parse_model POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/test/resources/models/test.obj ${PROJECT_BINARY_DIR}/test/resources/models)

add_custom_command(TARGET test_parse_model_big POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/test/resources/models/big.obj ${PROJECT_BINARY_DIR}/test/resources/models)

if(${GLGE_DRIVER} STREQUAL "OPENGL")
	add_subdirectory(opengl)
endif()
