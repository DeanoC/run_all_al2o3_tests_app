cmake_minimum_required(VERSION 3.12.4)
get_directory_property(hasParent PARENT_DIRECTORY)
if(NOT hasParent)
	option(unittests "unittests" OFF)
	get_filename_component(_PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
	set_property(GLOBAL PROPERTY GLOBAL_FETCHDEPS_BASE ${_PARENT_DIR}/al2o3 )
	include(FetchContent)
	FetchContent_Declare( al2o3 GIT_REPOSITORY https://github.com/DeanoC/al2o3 GIT_TAG master )
	FetchContent_GetProperties(al2o3)
	if(NOT al2o3_POPULATED)
		FetchContent_Populate(al2o3)
		add_subdirectory(${al2o3_SOURCE_DIR} ${al2o3_BINARY_DIR})
	endif()
	INIT_AL2O3(${CMAKE_CURRENT_SOURCE_DIR})
endif ()

SET_MIN_VERSIONS()
set(ProjectName run_all_al2o3_tests_app)
project(${ProjectName})

set(Src main.cpp)
set(Deps
		al2o3_platform
		al2o3_os
		utils_simple_logmanager
		)

set(TestsToBuild
		al2o3_platform
		al2o3_memory
		al2o3_cadt
		al2o3_catch2
		al2o3_tinystl
		al2o3_vfile
		al2o3_os
		al2o3_cmath
		al2o3_lz4
		al2o3_miniz
		al2o3_stb
		al2o3_syoyo
		al2o3_enki
		data_bufferutils
		data_binify
		data_binnybundle
		data_binnybundlewriter
		data_resourcemanager
		utils_simple_logmanager
		utils_misccpp
		utils_fmt
		tiny_ktx
		tiny_dds
		tiny_imageformat
		gfx_image_interface
		gfx_image_impl_basic
		gfx_imageio
		gfx_imagecompress
		gfx_imagedecompress
		gfx_meshmod
		gfx_meshmodio
		gfx_spirvheaders
		gfx_spirvcross
		gfx_glslang
		gfx_spirvtools
		gfx_shaderc
		gfx_shadercompiler
		gfx_imgui
		lua_base5.3
		lua_image
		lua_al2o3_os
		base_pxfoundations
		physics_physx4
		)

# current linux is command line only
if(APPLE OR WIN32)
list( APPEND TestsToBuild
		input_basic_interface
		utils_gameappshell_interface
		gfx_theforge
		input_gainput
		utils_gameappshell_impl_basic
		input_basic_impl_gainput
		gfx_imgui_al2o3_theforge_bindings
		utils_nativefiledialogs
)
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
set(_deps ${TestsToBuild})
set(_deptests ${TestsToBuild})
list(TRANSFORM _deptests PREPEND test_ )

foreach (_dep ${_deps})
	get_filename_component(deplibname ${_dep} NAME)
	FETCH_DEPENDENCY(${deplibname})
endforeach()

foreach (_dep ${_deptests})
	get_filename_component(deplibname ${_dep} NAME)
	if(TARGET ${deplibname})
		add_dependencies(${ProjectName} ${deplibname})
	endif()
endforeach()
