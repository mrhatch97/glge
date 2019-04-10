target_sources(glge
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/input/input_controller.cpp
		${CMAKE_CURRENT_LIST_DIR}/proc_gen/terrain.cpp
        #${CMAKE_CURRENT_LIST_DIR}/proc_gen/tree.cpp
)

target_include_directories(glge
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/proc_gen
)
