cmake_minimum_required(VERSION 2.8)

find_package(Antlr3 REQUIRED)
include_directories(${ANTLR3_INCLUDE_DIR})

# add the command to generate the source code from ANTLR grammar
add_custom_command (
    WORKING_DIRECTORY 
        ${CMAKE_CURRENT_BINARY_DIR}
    OUTPUT 
        DisaLexer.cpp
        DisaParser.cpp
        DisaLexer.h
        DisaParser.h
        Disa.tokens
    DEPENDS 
        Disa.g
    COMMAND 
        antlr3 -o ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Disa.g
        && mv -f DisaLexer.c DisaLexer.cpp
        && mv -f DisaParser.c DisaParser.cpp
  )

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(disa DisaCodeGenerator.cpp 
    DisaInstruction.cpp    
    DisaLexer.cpp
    DisaParser.cpp
    DisaAssemblyParser.cpp
)

target_link_libraries(disa ${ANTLR3_LIBRARIES})
