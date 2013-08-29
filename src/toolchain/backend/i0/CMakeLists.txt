cmake_minimum_required(VERSION 2.8)

find_package(Antlr3 REQUIRED)
include_directories(${ANTLR3_INCLUDE_DIR})

# add the command to generate the source code from ANTLR grammar
add_custom_command (
    WORKING_DIRECTORY 
        ${CMAKE_CURRENT_BINARY_DIR}
    OUTPUT 
        I0Lexer.cpp
        I0Parser.cpp
        I0Lexer.h
        I0Parser.h
        I0.tokens
    DEPENDS 
        I0.g
    COMMAND 
        antlr3 -o ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/I0.g
        && mv -f I0Lexer.c I0Lexer.cpp
        && mv -f I0Parser.c I0Parser.cpp
  )

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(i0 I0CodeGenerator.cpp 
     I0Instruction.cpp    
     I0Lexer.cpp
     I0Parser.cpp
     I0AssemblyParser.cpp
)
 
target_link_libraries(i0 ${ANTLR3_LIBRARIES})
