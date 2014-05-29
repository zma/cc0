cmake_minimum_required(VERSION 2.8)

project(toolchain)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake.d/Modules")

## Compiler flags
# if(CMAKE_COMPILER_IS_GNUCXX)
#    set(CMAKE_CXX_FLAGS "-g -static -static-libstdc++ -static-libgcc")        ## Optimize
#    # set(CMAKE_CXX_FLAGS "-g")
#    # set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
#    set(CMAKE_EXE_LINKER_FLAGS "")  ## Strip binary
#endif()
if(UNIX)
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so)
elseif(WIN32)
	set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll)
elseif(APPLE)
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so) 
else()
	error("Platform is not supported.")
endif()

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(frontend)
add_subdirectory(backend)
add_subdirectory(binary)
add_subdirectory(tools)

add_subdirectory(optimization)
