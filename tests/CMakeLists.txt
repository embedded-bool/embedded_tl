#include(FetchContent)
#
## Get Catch2 git hub project.
#FetchContent_Declare(
#        Catch2
#        GIT_REPOSITORY ${CATCH2_GITHUB}
#        GIT_TAG ${CATCH2_TAG}
#)
#
#FetchContent_MakeAvailable(Catch2)
#
#list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
#include(CTest)
#include(Catch)
#catch_discover_tests(tests)

message(DEBUG "Catch 2 source dir : ${catch2_SOURCE_DIR}")

set(UNIT_TEST_INC_DIRS
        inc
)

file(GLOB_RECURSE UNIT_TEST_FILES
        src/*.cpp
)

message(DEBUG "Unit Test source file: ${UNIT_TEST_FILES}.")

# Create Unit Test executable.
set(UNIT_TEST_NAME "embtl-unit-test")
add_executable(${UNIT_TEST_NAME}.exe ${UNIT_TEST_FILES})
target_include_directories(${UNIT_TEST_NAME}.exe
        PUBLIC ${EMBEDDED_TL_INC_DIR}
        PRIVATE ${UNIT_TEST_INC_DIRS}
)
target_link_libraries(${UNIT_TEST_NAME}.exe PRIVATE "Catch2::Catch2WithMain")

enable_testing()
catch_discover_tests(${UNIT_TEST_NAME}.exe)
