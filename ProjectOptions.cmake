include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

macro(SabaOptionsSetup)
    option(SabaWarningsAsErrors "Treat Warnings As Errors" OFF)
    option(SabaEnableClangTidy "Enable clang-tidy" ON)
    option(SabaEnableCppCheck "Enable cpp-check analysis" OFF)
    option(SabaEnablePCH "Enable precompiled headers" ON)

    mark_as_advanced(
        SabaWarningsAsErrors
        SabaEnableClangTidy
        SabaEnableCppCheck
        SabaEnablePCH
    )
endmacro()

macro(SabaApplyOptions)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    add_library(SabaWarnings INTERFACE)
    add_library(SabaOptions INTERFACE)

    if (NOT ${SabaWarningsAsErrors})
        message("###################################################################")
        message("#   Warning: compiler warnings not treated as errors              #")
        message("#   It is recommended to set this option to ON                    #")
        message("###################################################################")
    endif()

    include(Cmake/CompilerWarnings.cmake)
    SetProjectWarnings(SabaWarnings ${SabaWarningsAsErrors} "" "" "" "")

    include(Cmake/ClangTidy.cmake)
    if (SabaEnableClangTidy)
        EnableClangTidy(SabaOptions ${SabaWarningsAsErrors})
    endif()

    include(Cmake/CppCheck.cmake)
    if (SabaEnableCppCheck)
        EnableCppCheck(${SabaWarningsAsErrors} "")
    endif()

    if (SabaEnablePCH)
        target_precompile_headers(SabaOptions INTERFACE "<pch.h>")
    endif()
endmacro()
