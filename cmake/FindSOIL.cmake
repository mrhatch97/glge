# Find SOIL
# Find the SOIL includes and library
#
#  SOIL_INCLUDE_DIRS - where to find SOIL.h, etc.
#  SOIL_LIBRARIES    - List of libraries when using SOIL.
#  SOIL_FOUND        - True if SOIL found.
#
#  SOIL::SOIL        - Imported target
#
#  Findmodule based on https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/#if-you-want-it-done-right-do-it-yourself

find_package(PkgConfig QUIET)
pkg_check_modules(PC_SOIL QUIET SOIL)

find_path(SOIL_INCLUDE_DIR SOIL.h 
	PATHS ${PC_SOIL_INCLUDE_DIRS}
	PATH_SUFFIXES SOIL include/SOIL)

find_library(SOIL_LIBRARY 
	NAMES SOIL Soil soil
	PATHS ${PC_SOIL_LIBRARY})

set(SOIL_VERSION ${PC_SOIL_VERSION})

if(NOT PC_SOIL_LIBRARY AND NOT SOIL_LIBRARY)
	find_library(SOIL_LIBRARY_RELEASE 
		NAMES SOIL Soil soil)
	find_library(SOIL_LIBRARY_DEBUG
		NAMES SOILd Soild soild)

	include(SelectLibraryConfigurations)
	select_library_configurations(SOIL)
endif()

mark_as_advanced(SOIL_FOUND SOIL_INCLUDE_DIR SOIL_LIBRARY SOIL_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SOIL
	REQUIRED_VARS SOIL_INCLUDE_DIR SOIL_LIBRARY
	VERSION_VAR SOIL_VERSION
)

if(SOIL_FOUND)
	set(SOIL_INCLUDE_DIRS ${SOIL_INCLUDE_DIR})

	if(NOT TARGET SOIL::SOIL)
		add_library(SOIL::SOIL STATIC IMPORTED)
		set_target_properties(SOIL::SOIL PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${SOIL_INCLUDE_DIR}")

		if(SOIL_LIBRARY_RELEASE)
			set_property(TARGET SOIL::SOIL APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
			set_target_properties(SOIL::SOIL PROPERTIES IMPORTED_LOCATION_RELEASE "${SOIL_LIBRARY_RELEASE}")
		endif()

		if(SOIL_LIBRARY_DEBUG)
			set_property(TARGET SOIL::SOIL APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
			set_target_properties(SOIL::SOIL PROPERTIES IMPORTED_LOCATION_DEBUG "${SOIL_LIBRARY_DEBUG}")
		endif()

		if(NOT SOIL_LIBRARY_RELEASE AND NOT SOIL_LIBRARY_DEBUG)
			set_property(TARGET SOIL::SOIL APPEND PROPERTY IMPORTED_LOCATION "${SOIL_LIBRARY}")
		endif()
	endif()
endif()
