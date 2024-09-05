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

if (${SYSTEM_BUILD_TYPE} STREQUAL "UnitTest")
    add_subdirectory(tests)
endif ()

add_custom_target(run_local_docker
        COMMAND docker build -t embtl-local-image --file ./tools/docker/local.docker .
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "builds docker image from local.docker file."
)

add_custom_target(run_github_docker
        COMMAND docker build -t embtl-github-image --file ./tools/docker/github.docker .
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "builds image for github.docker file."
)
