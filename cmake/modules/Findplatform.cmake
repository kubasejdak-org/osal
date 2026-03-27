include(FetchContent)
FetchContent_Declare(platform
    GIT_REPOSITORY  https://github.com/kubasejdak-org/platform.git
    GIT_TAG         3274ae775a5e26a084c6ad2cc776a1863d631c23
    SOURCE_SUBDIR   lib
)

FetchContent_MakeAvailable(platform)
include(${platform_SOURCE_DIR}/cmake/components.cmake)
