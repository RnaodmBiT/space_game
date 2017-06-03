cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(space)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)

include_directories(common
                    thirdparty/stb-master
                    thirdparty/SDL2-2.0.4/include
                    thirdparty/glew)

set(SDL_STATIC ON)
set(SDL_SHARED OFF)
add_subdirectory(thirdparty/SDL2-2.0.4)
add_subdirectory(thirdparty/glew)


file(GLOB_RECURSE COMMON_SRC common/*.cpp common/*.h*)
add_library(common ${COMMON_SRC})



file(GLOB_RECURSE GAME_SRC game/*.cpp game/*.h*)
add_executable(game ${GAME_SRC})
target_link_libraries(game common SDL2-static SDL2main glew ${OPENGL_LIBRARIES})
