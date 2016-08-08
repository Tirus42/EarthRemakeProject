cmake_minimum_required (VERSION 2.6)

project (LibFF)


include_directories("include")
include_directories("${EARTH2150_PATH}/include")

add_library(LibFF STATIC
	src/ff/JumpPointSearch.cpp
)

