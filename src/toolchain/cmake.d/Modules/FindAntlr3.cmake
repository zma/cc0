
if (ANTLR3_INCLUDE_DIR AND ANTLR3_LIBRARIES)
    set(ANTLR3_FOUND TRUE)
else (ANTLR3_INCLUDE_DIR AND ANTLR3_LIBRARIES)
    find_path(ANTLR3_INCLUDE_DIR antlr3.h)
    find_library(ANTLR3_LIBRARIES NAMES antlr3 antlr3c libantlr3 libantlr3c)
    if (ANTLR3_INCLUDE_DIR AND ANTLR3_LIBRARIES)
        set(ANTLR3_FOUND TRUE)
        # TODO version check is missing
    endif (ANTLR3_INCLUDE_DIR AND ANTLR3_LIBRARIES)	

    if (ANTLR3_FOUND)
        if (NOT ANTLR3_FIND_QUIETLY)
            message(STATUS "Found Antlr3 C runtime: ${ANTLR3_LIBRARIES}")
        endif (NOT ANTLR3_FIND_QUIETLY)
    else (ANTLR3_FOUND)
        if (ANTLR3_FIND_REQUIRED)
            if (NOT ANTLR3_INCLUDE_DIR)
                message(FATAL_ERROR "Could NOT find Antlr3 header files")
            endif (NOT ANTLR3_INCLUDE_DIR)
            if (NOT ANTLR3_LIBRARIES)
                message(FATAL_ERROR "Could NOT find Antlr3 C runtime library")
            endif (NOT ANTLR3_LIBRARIES)
        endif (ANTLR3_FIND_REQUIRED)
    endif (ANTLR3_FOUND)
    mark_as_advanced(ANTLR3_INCLUDE_DIR ANTLR3_LIBRARIES)
endif (ANTLR3_INCLUDE_DIR AND ANTLR3_LIBRARIES)
