cmake_minimum_required(VERSION 2.8)

project(toolchain)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake.d/Modules")

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-static -static-libstdc++ -static-libgcc")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
endif()

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(frontend)
add_subdirectory(backend)
add_subdirectory(binary)
add_subdirectory(tools)

add_subdirectory(optimization)
