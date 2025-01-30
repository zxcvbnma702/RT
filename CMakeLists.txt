cmake_minimum_required(VERSION 3.5.0)
project(RT VERSION 0.1.0 LANGUAGES C CXX)

set(MY_LIBRARY_DIR /Library/Vulkan/macOS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# third library
set(SDL_INCLUDE_DIR ${MY_LIBRARY_DIR}/include/SDL2)
set(SDL_INCLUDE_LIB ${MY_LIBRARY_DIR}/lib)

include_directories(${SDL_INCLUDE_DIR})
link_directories(${SDL_INCLUDE_LIB})

link_libraries(SDL2-2.0.0)

# library
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${SDL_LIB_DIR})

set(SOURCE_FILES
    src/main.cpp
    src/CApp.cpp
)

add_executable(RT ${SOURCE_FILES})

set_target_properties(RT PROPERTIES
    INSTALL_RPATH ${MY_LIBRARY_DIR}/lib
)