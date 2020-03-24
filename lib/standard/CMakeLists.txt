add_library(osal-standard EXCLUDE_FROM_ALL
    init.cpp
    sleep.cpp
    timestamp.cpp
)

if (UNIX)
    target_sources(osal-standard
        PRIVATE threadLinux.cpp
    )
endif ()

target_include_directories(osal-standard
    PUBLIC ../include include
)
