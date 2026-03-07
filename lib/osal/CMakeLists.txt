cmake_minimum_required(VERSION 3.28)

include(${osal_SOURCE_DIR}/cmake/compilation-flags.cmake)

if (NOT DEFINED OSAL_PLATFORM)
    message(FATAL_ERROR "osal: 'OSAL_PLATFORM' is not defined!")
endif ()

add_library(osal-c EXCLUDE_FROM_ALL)
add_library(osal::c ALIAS osal-c)

target_include_directories(osal-c
    PUBLIC
        include
        ${OSAL_PLATFORM}/include
)

target_link_libraries(osal-c
    PRIVATE
        osal::common
)

add_subdirectory(${OSAL_PLATFORM})
add_subdirectory(common)
