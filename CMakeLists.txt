cmake_minimum_required (VERSION 3.12)

project(glge
	VERSION 0.1.0.0
	LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(glm REQUIRED)
find_package(SOIL REQUIRED)
if(WIN32)
	# smh
	list(APPEND CMAKE_PREFIX_PATH "C:/Program Files/GLEW")
	list(APPEND CMAKE_LIBRARY_PATH "C:/Program Files/GLEW/lib/Release/x64/")
	find_package(GLEW REQUIRED)
elseif(APPLE)
	find_library(COCOA Cocoa)
    if(NOT COCOA)
            message(FATAL_ERROR "Cocoa not found")
    endif()

    find_library(OPENGL OpenGL)
    if(NOT OPENGL)
            message(FATAL_ERROR "OpenGL not found")
    endif()

    find_library(IOKIT IOKit)
    if(NOT IOKIT)
            message(FATAL_ERROR "IOKit not found")
    endif()

    find_library(COREVIDEO CoreVideo)
    if(NOT COREVIDEO)
            message(FATAL_ERROR "CoreVideo not found")
    endif()
endif()

add_subdirectory(src/Support)
add_subdirectory(src/Util)
add_subdirectory(src/Renderer)

add_library (glge STATIC
	$<TARGET_OBJECTS:support>
	$<TARGET_OBJECTS:util>
	$<TARGET_OBJECTS:render>
	src/Common.cpp
)

target_link_libraries(glge
	PUBLIC
		glm
	PRIVATE
		SOIL
)

# Platform-specific dependencies
if(APPLE)
	target_link_libraries(glge
		PRIVATE
			${COCOA}
			${OPENGL}
			${IOKIT}
			${COREVIDEO}
	)
elseif(UNIX)
	target_link_libraries(glge
		PRIVATE
			pthread
			dl
			GLU
			GL
			rt
			Xrandr
			Xxf86vm
			Xi
			Xinerama
			X11
	)
elseif(WIN32)
	target_link_libraries(glge
		PRIVATE
			GLEW::GLEW
	)

	# TODO switch to install TARGETS with cmake 3.13
	#install(FILES ${glew_SOURCE_DIR}/bin/Release/x64/glew32.dll
	#	DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

target_include_directories(glge 
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

target_compile_features(glge 
	PUBLIC 
		cxx_std_17
)

# Set appropriate debug flags for build type
target_compile_definitions(glge
	PRIVATE
		$<$<CONFIG:Debug>:_DEBUG>
		$<$<CONFIG:RelWithDebInfo>:_DEBUG>
		$<$<CONFIG:Release>:NDEBUG>
		$<$<CONFIG:MinSizeRel>:NDEBUG>
		GLM_FORCE_SILENT_WARNINGS
)

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