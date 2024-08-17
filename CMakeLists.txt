cmake_minimum_required(VERSION 3.24)

project("Embedded Template Library"
        LANGUAGES CXX
        VERSION 1.0.0
        DESCRIPTION "Embedded Template Library for microcontroller."
)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

include(tools/cmake/compiler_default_warnings.cmake)

if (${SYSTEM_BUILD_TYPE} STREQUAL "UnitTest")
    include(tools/cmake/dep_catch2.cmake)
endif ()

add_subdirectory(embedded)
add_subdirectory(tests)
