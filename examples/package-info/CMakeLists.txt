add_executable(osal-package-info-example
    main.cpp
)

find_package(osal COMPONENTS package)
find_package(platform COMPONENTS main)

target_link_libraries(osal-package-info-example
    PRIVATE
        osal::package
        platform::init
        platform::main
)

if (NOT UNIX)
    objcopy_generate_bin(osal-package-info-example)
endif ()

install(TARGETS osal-package-info-example)
