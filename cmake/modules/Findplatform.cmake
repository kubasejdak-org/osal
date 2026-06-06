include(FetchContent)
FetchContent_Declare(platform
    GIT_REPOSITORY  https://github.com/kubasejdak-org/platform.git
    GIT_TAG         bff49f6cadfec2c227f9ce23746bfd1511e04c85
    SOURCE_SUBDIR   lib
)

FetchContent_MakeAvailable(platform)
include(${platform_SOURCE_DIR}/cmake/components.cmake)
