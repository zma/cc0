cmake_minimum_required(VERSION 2.8)

find_package(Antlr3 REQUIRED)
include_directories(${ANTLR3_INCLUDE_DIR})

# add the command to generate the source code from ANTLR grammar
add_custom_command (
    WORKING_DIRECTORY 
        ${CMAKE_CURRENT_BINARY_DIR}
    OUTPUT 
        CLexer.cpp
        CParser.cpp
        CLexer.h
        CParser.h
        C.tokens
    DEPENDS 
        C.g
    COMMAND 
        antlr3 -o ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/C.g
        && mv -f CLexer.c CLexer.cpp
        && mv -f CParser.c CParser.cpp
  )

include_directories(${CMAKE_CURRENT_BINARY_DIR})


add_library(langc CSourceParser.cpp DeclarationTemplate.cpp
    CLexer.cpp
    CParser.cpp
  )

add_dependencies(langc core)
target_link_libraries(langc core)
target_link_libraries(langc ${ANTLR3_LIBRARIES})

