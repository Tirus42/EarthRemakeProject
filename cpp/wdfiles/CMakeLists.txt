cmake_minimum_required (VERSION 2.6)

project (WDTest)


add_executable(WDTest
	wdtest.cpp
)

target_link_libraries(WDTest z)
