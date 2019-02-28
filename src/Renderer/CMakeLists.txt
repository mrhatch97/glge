# Based on https://stackoverflow.com/a/47801116
# Allows including the content of arbitrary files in a 
# C++ string at compile time
function(make_includable infile outfile)
	file(READ ${infile} content)
	set(delim "TEXT_INCLUDE")
	set(content "R\"${delim}(\n${content})${delim}\"")
	file(WRITE ${outfile} "${content}")
endfunction(make_includable)

add_library(render OBJECT
	Renderer.cpp
	RenderSettings.cpp
	Camera.cpp
	Engine.cpp
	Primitives/Cubemap.cpp
	Primitives/Lines.cpp
	Primitives/ShaderProgram.cpp
	Primitives/Texture.cpp
	Resources/ModelLoader.cpp
	Resources/ResourceManager.cpp
	SceneGraph/DynamicTransform.cpp
	SceneGraph/Node.cpp
	SceneGraph/Scene.cpp
	SceneGraph/SceneCamera.cpp
	SceneGraph/StaticTransform.cpp
	SceneGraph/Traversal.cpp

	# OpenGL Support
	Primitives/OpenGL/GLCommon.cpp
	Primitives/OpenGL/GLCubeMap.cpp
	Primitives/OpenGL/GLLines.cpp
	Primitives/OpenGL/GLModel.cpp
	Primitives/OpenGL/GLShader.cpp
	Primitives/OpenGL/GLTexture.cpp
)

target_include_directories(render
	PUBLIC 
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		Primitives/OpenGL
)

target_compile_features(render
	PUBLIC
		cxx_std_17)

target_compile_definitions(render
	PRIVATE
		$<$<CONFIG:Debug>:_DEBUG>
		$<$<CONFIG:RelWithDebInfo>:_DEBUG>
		$<$<CONFIG:Release>:NDEBUG>
		$<$<CONFIG:MinSizeRel>:NDEBUG>
		GLM_FORCE_SILENT_WARNINGS
)

target_link_libraries(render
	PUBLIC 
		glm
	PRIVATE
		GLEW::GLEW
		SOIL
)

# Generate delimited GLSL code for inclusion in C++ at compile-time
file(GLOB GLSL_SOURCES 
	RELATIVE ${PROJECT_SOURCE_DIR}/src/Renderer/Primitives/OpenGL
	${PROJECT_SOURCE_DIR}/src/Renderer/Primitives/OpenGL/glsl/*.glsl)

foreach(_file ${GLSL_SOURCES})
	make_includable(${PROJECT_SOURCE_DIR}/src/Renderer/Primitives/OpenGL/${_file} ${PROJECT_BINARY_DIR}/generated/include/generated/${_file})
endforeach(_file)

target_include_directories(render
	PRIVATE
		${PROJECT_BINARY_DIR}/generated/include
)