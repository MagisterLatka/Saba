function(SabaPreventInSourceIntermediates)
    get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
    get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

    if ("${srcdir}" STREQUAL "${bindir}")
        message("##################################################################")
        message("Error: intermediate files inside source folder are forbidden")
        message("Create a separate directory for intermediate files and run cmake from there")
        message(FATAL_ERROR "Forbidden intermediate target directory")
    endif()
endfunction()

SabaPreventInSourceIntermediates()
