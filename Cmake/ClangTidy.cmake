macro(EnableClangTidy target warningsAsErrors)
    find_program(clangTidy clang-tidy)
    if (clangTidy)
        set(clangTidyOptions ${clangTidy}
            -extra-arg=-Wno-unknown-warning-option
            -extra-arg=-Wno-ignored-optimization-argument
            -extra-arg=-Wno-unused-command-line-argument
            -p
        )

        if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
            if ("${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl")
                list(APPEND clangTidyOptions -extra-arg=/std:c++${CMAKE_CXX_STANDARD})
            else()
                list(APPEND clangTidyOptions -extra-arg=-std:c++${CMAKE_CXX_STANDARD})
            endif()
        endif()

        if (${warningsAsErrors})
            list(APPEND clangTidyOptions -warnings-as-errors=*)
        endif()

        set(CMAKE_CXX_CLANG_TIDY ${clangTidyOptions})
        message(${WARNING_MESSAGE} "clang-tidy set up")
    else()
        message(${WARNING_MESSAGE} "clang-tidy requested but executable not found")
    endif()
endmacro()
