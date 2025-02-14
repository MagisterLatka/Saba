macro(EnableCppCheck warningsAsErrors cppCheckOptions)
    find_program(cppCheck cppcheck)
    if (cppCheck)
        if (CMAKE_GENERATOR MATCHES ".*Visual Studio.*")
            set(cppCheckTemplate "vs")
        else()
            set(cppCheckTemplate "gcc")
        endif()

        if ("${cppCheckOptions}" STREQUAL "")
            set(CMAKE_CXX_CPPCHECK ${cppCheck}
                --template=${cppCheckTemplate}
                --enable=style,performance,warning,portability
                --inline-suppr
                --suppress=cppcheckError
                --suppress=internalAstError
                --suppress=unmatchedSuppression
                --suppress=passedByValue
                --suppress=syntaxError
                --suppress=preprocessorErrorDirective
                --inconclusive
            )
        else()
            set(CMAKE_CXX_CPPCHECK ${cppCheck} --template=${cppCheckTemplate} ${cppCheckOptions})
        endif()

        if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
            list(APPEND CMAKE_CXX_CPPCHECK --std=c++${CMAKE_CXX_STANDARD})
        endif()

        if (${warningsAsErrors})
            list(APPEND CMAKE_CXX_CPPCHECK --error-exitcode=2)
        endif()
    else()
        message(${WARNING_MESSAGE} "cppcheck requested but executable not found")
    endif()
endmacro()
