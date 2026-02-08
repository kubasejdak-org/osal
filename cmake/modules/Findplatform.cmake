include(FetchContent)
FetchContent_Declare(platform
    GIT_REPOSITORY  https://github.com/kubasejdak-org/platform.git
    GIT_TAG         dea2a14ea9acdde6a258b055b6b5a9ec0791e7b9
    SOURCE_SUBDIR   lib
)

FetchContent_MakeAvailable(platform)
include(${platform_SOURCE_DIR}/cmake/components.cmake)
