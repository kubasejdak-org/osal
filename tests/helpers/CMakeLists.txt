add_library(tests-helpers INTERFACE)
add_library(tests::helpers ALIAS tests-helpers)

target_include_directories(tests-helpers
    INTERFACE
        include
)

find_package(Catch2)

target_link_libraries(tests-helpers
    INTERFACE
        Catch2::Catch2
)
