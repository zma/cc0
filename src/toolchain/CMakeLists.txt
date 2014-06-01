cmake_minimum_required(VERSION 2.8)

project(toolchain)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake.d/Modules")

if(UNIX)
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so)
elseif(WIN32)
	set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll)
elseif(APPLE)
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so) 
else()
	message(WARNING, "Platform is not supported.")
endif()

# USE variable CC0_USE_STATIC_CRT to control whether to link against static
# version of C/C++ runtime library (libc on UNIX-based systems, and VC++
# Runtime Redistributes on Windows)

if(CC0_USE_STATIC_CRT)
	if(CMAKE_COMPILER_IS_GNUCC)
		foreach(flag
			CMAKE_C_FLAGS
			CMAKE_C_FLAGS_RELEASE
			CMAKE_C_FLAGS_RELWITHDEBINFO
			CMAKE_C_FLAGS_MINSIZEREL
			CMAKE_C_FLAGS_DEBUG)
			set("${flag}" "${${flag}} -static -static-libgcc")
		endforeach()
	endif()
	
	if(CMAKE_COMPILER_IS_GNUCXX)
		foreach(flag
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_RELWITHDEBINFO
			CMAKE_CXX_FLAGS_MINSIZEREL
			CMAKE_CXX_FLAGS_DEBUG)
			set(${flag} "${${flag}} -static -static-libgcc -static-libstdc++")
		endforeach()
	endif()
	
	if(CMAKE_COMPILER_IS_MSVC)
		foreach(flag
			CMAKE_C_FLAGS
			CMAKE_C_FLAGS_RELEASE
			CMAKE_C_FLAGS_RELWITHDEBINFO
			CMAKE_C_FLAGS_MINSIZEREL
			CMAKE_C_FLAGS_DEBUG
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_RELWITHDEBINFO
			CMAKE_CXX_FLAGS_MINSIZEREL
			CMAKE_CXX_FLAGS_DEBUG)
			STRING(REPLACE "/MD"  "/MT" "${flag}" "${${flag}}")
		endforeach()
	endif()
endif()

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(frontend)
add_subdirectory(backend)
add_subdirectory(binary)
add_subdirectory(tools)

add_subdirectory(optimization)
