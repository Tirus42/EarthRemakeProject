cmake_minimum_required (VERSION 2.6)

project (LibTF)


include_directories("include")

add_library(LibTF STATIC
	src/tf/time.cpp
	src/tf/network.cpp
	src/tf/file.cpp
	src/tf/thread.cpp
)

