cmake_minimum_required(VERSION 3.5.0)
project(RT VERSION 0.1.0 LANGUAGES C CXX)

set(MY_LIBRARY_DIR /Library/Vulkan/macOS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SDL_INCLUDE_DIR ${MY_LIBRARY_DIR}/include/SDL2)
set(SDL_INCLUDE_LIB ${MY_LIBRARY_DIR}/lib)

include_directories(${SDL_INCLUDE_DIR})
link_directories(${SDL_INCLUDE_LIB})

link_libraries(SDL2-2.0.0)

add_executable(RT main.cpp)

