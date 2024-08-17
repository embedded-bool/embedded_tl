    include(FetchContent)

    # Get Catch2 git hub project. (URL and Tag are system cache variables declared in CMakePresets.json.
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY ${CATCH2_GITHUB}
            GIT_TAG ${CATCH2_TAG}
    )

    FetchContent_MakeAvailable(Catch2)

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(CTest)
    include(Catch)
