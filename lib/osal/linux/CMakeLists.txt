target_sources(osal-c PRIVATE
    Mutex.cpp
    Semaphore.cpp
    sleep.cpp
    Thread.cpp
    timestamp.cpp
)

target_link_libraries(osal-c
    PRIVATE
        pthread
)
