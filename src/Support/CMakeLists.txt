add_library(support OBJECT
	Input/InputController.cpp
	ProcGen/DiamondSquare.cpp
	ProcGen/LTurtle.cpp
	ProcGen/Terrain.cpp
	ProcGen/TerrainMesh.cpp
	ProcGen/Tree.cpp
)

target_include_directories(support
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		ProcGen
)

target_compile_features(support
	PUBLIC
		cxx_std_17
)

target_compile_definitions(support
	PRIVATE
		$<$<CONFIG:Debug>:_DEBUG>
		$<$<CONFIG:RelWithDebInfo>:_DEBUG>
		$<$<CONFIG:Release>:NDEBUG>
		$<$<CONFIG:MinSizeRel>:NDEBUG>
		GLM_FORCE_SILENT_WARNINGS
)

target_link_libraries(support
	PUBLIC
		glm
)
