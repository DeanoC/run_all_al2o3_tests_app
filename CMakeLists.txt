cmake_minimum_required(VERSION 3.13)
set(ProjectName run_all_al2o3_tests_app)
project(${ProjectName})

set(Src main.cpp)
set(Deps
		al2o3_platform
		al2o3_catch2
		al2o3_tinystl
		al2o3_os
		al2o3_sstl
		al2o3_cmath
		al2o3_lz4
		al2o3_miniz
		)

include(FetchContent)
FetchContent_Declare( al2o3 GIT_REPOSITORY https://github.com/DeanoC/al2o3 GIT_TAG master )
FetchContent_GetProperties(al2o3)
if(NOT al2o3_POPULATED)
	FetchContent_Populate(al2o3)
	add_subdirectory(${al2o3_SOURCE_DIR} ${al2o3_BINARY_DIR})
endif()

option(BUILD_TESTS "Build Test executubles" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out_libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out_bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out_bin)

set(LIB_BASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/libs/)

ADD_CONSOLE_APP(${ProjectName} "${Src}" "${Deps}")

# this magic incantation ensure all test exes are created before this app
# so it can iterate and run them all, not all projects have test exes so
# this skips them for you
set(_deps ${Deps})
list(TRANSFORM _deps PREPEND test_ )
foreach (_dep ${_deps})
	get_filename_component(deplibname ${_dep} NAME)
	if(TARGET ${deplibname})
		add_dependencies(${ProjectName} ${deplibname})
	endif()
endforeach()