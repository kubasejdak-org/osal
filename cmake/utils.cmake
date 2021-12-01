include(FetchContent)
FetchContent_Declare(utils
    GIT_REPOSITORY  https://gitlab.com/embeddedlinux/libs/utils.git
    GIT_TAG         maintenance/improve-conan-support
)

FetchContent_GetProperties(utils)
if (NOT utils_POPULATED)
    FetchContent_Populate(utils)
endif ()
