

function(group SRC)
    #message(STATUS "SRC = ${SRC}")
    foreach(FILE ${${SRC}})
        get_filename_component(PARENT_DIR "${FILE}" PATH)

        # skip src or include and changes /'s to \\'s
        string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" GROUP "${PARENT_DIR}")
        string(REPLACE "/" "\\\\" GROUP "${GROUP}")
        string(REGEX REPLACE "^\\\\" "" GROUP "${GROUP}")

        source_group("${GROUP}" FILES "${FILE}")

        #message(STATUS "${FILE} getting grouped in ${GROUP}")
    endforeach()

endfunction(group)
