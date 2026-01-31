include(FetchContent)
FetchContent_Declare(platform
    GIT_REPOSITORY  https://github.com/kubasejdak-org/platform.git
    GIT_TAG         78496df2ce6a6e4aa54aa89cb58e97ee21e2d8e7
    SOURCE_SUBDIR   lib
)

FetchContent_MakeAvailable(platform)
include(${platform_SOURCE_DIR}/cmake/components.cmake)
