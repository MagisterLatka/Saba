function(SetProjectWarnings projectName warningsAsErrors msvcWarnings clangWarnings gccWarnings cudaWarnings)
    if("${msvcWarnings}" STREQUAL "")
        set(msvcWarnings
            /W4 # Baseline reasonable warnings
            /w14242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
            /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
            /w14263 # 'function': member function does not override any base class virtual member function
            /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not
            # be destructed correctly
            /w14287 # 'operator': unsigned/negative constant mismatch
            /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside
            # the for-loop scope
            /w14296 # 'operator': expression is always 'boolean_value'
            /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
            /w14545 # expression before comma evaluates to a function which is missing an argument list
            /w14546 # function call before comma missing argument list
            /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
            /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
            /w14555 # expression has no effect; expected expression with side- effect
            /w14619 # pragma warning: there is no warning number 'number'
            /w14640 # Enable warning on thread un-safe static member initialization
            /w14826 # Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
            /w14905 # wide string literal cast to 'LPSTR'
            /w14906 # string literal cast to 'LPWSTR'
            /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
            /permissive- # standards conformance mode for MSVC compiler.
            /Zc:preprocessor
        )
    endif()
    if("${clangWarnings}" STREQUAL "")
        set(clangWarnings
            -Wall
            -Wextra # reasonable and standard
            -Wshadow # warn the user if a variable declaration shadows one from a parent context
            -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps
            # catch hard to track down memory errors
            -Wold-style-cast # warn for c-style casts
            -Wcast-align # warn for potential performance problem casts
            -Wunused # warn on anything being unused
            -Woverloaded-virtual # warn if you overload (not override) a virtual function
            -Wpedantic # warn if non-standard C++ is used
            -Wconversion # warn on type conversions that may lose data
            -Wsign-conversion # warn on sign conversions
            -Wnull-dereference # warn if a null dereference is detected
            -Wdouble-promotion # warn if float is implicit promoted to double
            -Wformat=2 # warn on security issues around functions that format output (ie printf)
            -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
        )
    endif()
    if("${gccWarnings}" STREQUAL "")
        set(gccWarnings
            ${clangWarnings}
            -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
            -Wduplicated-cond # warn if if / else chain has duplicated conditions
            -Wduplicated-branches # warn if if / else branches have duplicated code
            -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
            -Wuseless-cast # warn if you perform a cast to the same type
        )
    endif()

    if("${cudaWarnings}" STREQUAL "")
        set(cudaWarnings
            -Wall
            -Wextra
            -Wunused
            -Wconversion
            -Wshadow
        )
    endif()

    if (warningsAsErrors)
        message(STATUS "WARNINGS TREATED AS ERRORS ENABLED")
        list(APPEND clangWarnings -Werror)
        list(APPEND gccWarnings -Werror)
        list(APPEND msvcWarnings /WX)
    endif()

    if(MSVC)
        set(projectWarningsCXX ${msvcWarnings})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        set(projectWarningsCXX ${clangWarnings})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(projectWarningsCXX ${gccWarnings})
    else()
        message(WARNING "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    endif()

    set(projectWarningsC "${projectWarningsCXX}")

	set(projectWarningsCuda "${cudaWarnings}")

    target_compile_options(${projectName} INTERFACE
        $<$<COMPILE_LANGUAGE:CXX>:${projectWarningsCXX}>
        $<$<COMPILE_LANGUAGE:C>:${projectWarningsC}>
        $<$<COMPILE_LANGUAGE:CUDA>:${projectWarningsCuda}>
    )
endfunction()
