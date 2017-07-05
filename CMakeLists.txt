cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

include(Functions.cmake)

project(space)
set(CMAKE_CXX_STANDARD 14)

if (MSVC)
    add_compile_options(-bigobj)
endif()

find_package(OpenGL REQUIRED)

add_subdirectory(thirdparty/SDL2-2.0.4)
add_subdirectory(thirdparty/glew)
add_subdirectory(thirdparty/assimp-3.3.1)

include_directories(common
                    thirdparty/stb
                    thirdparty/SDL2-2.0.4/include
                    thirdparty/glew
                    thirdparty/assimp-3.3.1/include)

# Add the common project
file(GLOB_RECURSE COMMON_SRC common/*.c common/*.cpp common/*.h common/*.hpp)
group(COMMON_SRC)
add_library(common ${COMMON_SRC})

# Add the game project
file(GLOB_RECURSE GAME_SRC game/*.c game/*.cpp game/*.h game/*.hpp)
add_executable(game ${GAME_SRC})
target_link_libraries(game common SDL2-static SDL2main glew assimp ${OPENGL_LIBRARIES})
group(GAME_SRC)

# Add the model_viewer project
file(GLOB_RECURSE MODEL_VIEWER_SRC model_viewer/*.c model_viewer/*.cpp model_viewer/*.h model_viewer/*.hpp)
add_executable(model_viewer ${MODEL_VIEWER_SRC})
target_link_libraries(model_viewer common SDL2-static SDL2main glew assimp ${OPENGL_LIBRARIES})
group(MODEL_VIEWER_SRC)

# Configure the template file
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(USER_FILE game.vcxproj.user)
set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
configure_file(template.vcxproj.user ${USER_FILE} @ONLY)

set(USER_FILE model_viewer.vcxproj.user)
set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
configure_file(template.vcxproj.user ${USER_FILE} @ONLY)
