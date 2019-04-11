cmake_minimum_required (VERSION 3.13)

project(glge
	VERSION 0.1.0.0
	LANGUAGES CXX
	DESCRIPTION "3D rendering engine library"
)

option(GLGE_HEADLESS_TESTS "Run OpenGL tests without an X server; requires EGL and GLEW compiled with EGL support" OFF)
set(GLGE_DRIVER "OPENGL" CACHE STRING "GLGE backend driver; currently only OPENGL supported")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(glm REQUIRED)

add_library (glge STATIC
	src/common.cpp
)

add_subdirectory(src/support)
add_subdirectory(src/util)
add_subdirectory(src/renderer)

target_link_libraries(glge
	PUBLIC
		glm
)

target_include_directories(glge
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

target_compile_features(glge
	PUBLIC
		cxx_std_17
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(glge
        PUBLIC
            $<$<CONFIG:Test>:--coverage -g -fPIC -O0> # Set up test flags for gcc test coverage report generation
        PRIVATE
            -Wall -Wextra -Werror
    )
    target_link_options(glge
        PUBLIC
            $<$<CONFIG:Test>:--coverage>
    )
endif()

target_compile_definitions(glge
	PRIVATE
        $<$<CONFIG:Debug>:GLGE_DEBUG>   # Enable debug info for Debug and Test builds
        $<$<CONFIG:Test>:GLGE_DEBUG>
        GLM_FORCE_SILENT_WARNINGS       # I don't care about warnings in glm's code
)

find_package(Doxygen)

option(BUILD_DOCUMENTATION "Build Doxygen documentation" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
	if(DOXYGEN_FOUND)
		set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
		set(DOXYGEN_OUT ${PROJECT_BINARY_DIR}/Doxyfile)

		configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
		message("Doxygen build enabled")

		add_custom_target(docs ALL
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
			COMMENT "Generating API docs with Doxygen"
			VERBATIM
		)

	else()
		message(FATAL_ERROR "Doxygen generation requested but package was not found")
	endif()
endif()

install(TARGETS glge EXPORT glgeConfig
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

write_basic_package_version_file(
	glgeConfigVersion.cmake
	VERSION ${PACKAGE_VERSION}
	COMPATIBILITY AnyNewerVersion
)

export(TARGETS glge
	FILE glgeConfig.cmake
	NAMESPACE glge::
)

install(EXPORT glgeConfig
	DESTINATION share/glge/cmake
)

include(CTest)

if(BUILD_TESTING)
	add_subdirectory(test)
endif()
