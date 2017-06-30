cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(space)
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-bigobj)

find_package(OpenGL REQUIRED)

add_subdirectory(thirdparty/SDL2-2.0.4)
add_subdirectory(thirdparty/glew)

include_directories(common
                    thirdparty/stb-master
                    thirdparty/SDL2-2.0.4/include
                    thirdparty/glew)

file(GLOB_RECURSE COMMON_SRC common/*.c common/*.cpp common/*.h common/*.hpp)


foreach(FILE ${COMMON_SRC})
    get_filename_component(PARENT_DIR "${FILE}" PATH)

    # skip src or include and changes /'s to \\'s
    string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\\\" GROUP "${GROUP}")
    string(REGEX REPLACE "^\\\\" "" GROUP "${GROUP}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

add_library(common ${COMMON_SRC})


file(GLOB_RECURSE GAME_SRC game/*.c game/*.cpp game/*.h game/*.hpp)
add_executable(game ${GAME_SRC})
target_link_libraries(game common SDL2-static SDL2main glew ${OPENGL_LIBRARIES})

foreach(FILE ${GAME_SRC})
    get_filename_component(PARENT_DIR "${FILE}" PATH)

    # skip src or include and changes /'s to \\'s
    string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\\\" GROUP "${GROUP}")
    string(REGEX REPLACE "^\\\\" "" GROUP "${GROUP}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

# Configure the template file
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(USER_FILE game.vcxproj.user)
set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
configure_file(template.vcxproj.user ${USER_FILE} @ONLY)
