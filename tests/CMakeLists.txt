add_subdirectory(helpers)
add_subdirectory(init)

add_executable(osal-tests
    # Error.cpp
    main.cpp
    # Mutex.cpp
    # MutexObject.cpp
    # ScopedLock.cpp
    # Semaphore.cpp
    # SemaphoreObject.cpp
    # Thread.cpp
    # ThreadObject.cpp
    # time.cpp
    # Timeout.cpp
    # timestamp.cpp
)

find_package(Catch2)
find_package(platform COMPONENTS main)

target_link_libraries(osal-tests
    PRIVATE
        Catch2::Catch2
        platform::init
        platform::main
        tests::helpers
)

# target_link_libraries(osal-tests
#     PRIVATE osal::c osal::cpp
# )

if (NOT UNIX)
    objcopy_generate_bin(osal-tests)
endif ()
