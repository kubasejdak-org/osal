if (NOT osal_FIND_COMPONENTS)
    file(GLOB osal_FIND_COMPONENTS LIST_DIRECTORIES true RELATIVE ${osal_SOURCE_DIR}/lib ${osal_SOURCE_DIR}/lib/*)
endif ()

include(FetchContent)
foreach (component IN LISTS osal_FIND_COMPONENTS)
    FetchContent_Declare(osal-${component}
        SOURCE_DIR      ${osal_SOURCE_DIR}
        SOURCE_SUBDIR   lib/${component}
        SYSTEM
    )

    FetchContent_MakeAvailable(osal-${component})
endforeach ()
