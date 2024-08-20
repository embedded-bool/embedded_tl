cmake_minimum_required(VERSION 3.24)

set(EMBEDDED_TL_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc CACHE INTERNAL "Embedded Template Library include directory")

if (${SYSTEM_BUILD_TYPE} STREQUAL "Build")

    if (NOT DEFINED EMBTL_LIB_NAME)
        set(EMBTL_LIB_NAME "embtl")
    endif ()

    if (DEFINED EMBTL_PREFIX)
        set(EMBTL_LIB_NAME ${EMBTL_PREFIX}-${EMBTL_LIB_NAME})
    endif ()

    if (DEFINED EMBTL_SUFFIX)
        set(EMBTL_LIB_NAME ${EMBTL_LIB_NAME}-${EMBTL_SUFFIX})
    endif ()

    file(GLOB_RECURSE EMBTL_HEADER_FILES
            inc/*.hpp
    )

    message(DEBUG "Header files : ${EMBTL_HEADER_FILES}")

    add_library(${EMBTL_LIB_NAME} INTERFACE)
    target_include_directories(${EMBTL_LIB_NAME} INTERFACE inc)

endif ()