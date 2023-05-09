include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_SUPPORTED_ERROR)

if(IPO_SUPPORTED)
    message(STATUS "IPO Enabled")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

    # Use full lto instead of the default thin lto
    if(NOT MSVC)
        add_compile_options(-flto)
        add_link_options(-flto)
    endif()
else()
    message(WARNING "IPO is not supported: ${IPO_SUPPORTED_ERROR}")
endif()
