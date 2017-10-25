cmake_minimum_required (VERSION 2.6)

SET(BUILD_SERVER TRUE CACHE BOOL "Build the server binary")
SET(BUILD_CLIENT TRUE CACHE BOOL "Build the client/gui")

SET(IRRLICHT_ENGINE_INCLUDE "" CACHE PATH "path to irrlicht engine include directory")
SET(IRRLICHT_ENGINE_LIB "" CACHE PATH "path to irrlicht engine library directory")


SET(EARTH2150_PATH "${CMAKE_CURRENT_SOURCE_DIR}/earth2150" CACHE PATH "Earth2150 dir")
SET(BINARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.." CACHE PATH "Output dir")

if (BUILD_CLIENT)
	if (IRRLICHT_ENGINE_INCLUDE STREQUAL "")
		MESSAGE("Irrlicht Engine include path not set!")
	endif()
	if (IRRLICHT_ENGINE_LIB STREQUAL "")
		MESSAGE("Irrlicht Engine library path not set!")
	endif()

	if (IRRLICHT_ENGINE_INCLUDE STREQUAL "" OR IRRLICHT_ENGINE_LIB STREQUAL "")
		MESSAGE(FATAL_ERROR "Please specify the Irrlicht Engine paths or disable the client build")
	endif()
endif()



include_directories("tf/include")
include_directories("ff/include")

add_subdirectory(tf)
add_subdirectory(ff)
add_subdirectory(earth2150)

if (BUILD_CLIENT)
	add_subdirectory(earth2150client)
endif()
