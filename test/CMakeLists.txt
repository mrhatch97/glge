message("glge tests enabled")

include_directories(
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/test/include>
)

link_libraries(glge)

add_executable(test_safe_cast
	test_safe_cast.cpp)

add_test(safe_cast
	test_safe_cast)

add_executable(test_returns_void
	test_returns_void.cpp)

add_test(returns_void
	test_returns_void)

add_executable(test_contains
	test_contains.cpp)

add_test(contains
	test_contains)

add_executable(test_stepper
	test_stepper.cpp)

add_test(stepper
	test_stepper)

add_executable(test_circular
	test_circular.cpp)

add_test(circular
	test_circular)

add_executable(test_oscillator
	test_oscillator.cpp)

add_test(oscillator
	test_oscillator)

add_executable(test_scene
	test_scene.cpp)

add_test(scene
	test_scene)

# Target to test that depends on the build of the test executables
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
	DEPENDS
		test_safe_cast
		test_returns_void
		test_contains
		test_stepper
		test_circular
		test_oscillator
		test_scene
)
