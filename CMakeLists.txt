cmake_minimum_required(VERSION 3.5.0)
project(RT VERSION 0.1.0 LANGUAGES C CXX)

set(MY_LIBRARY_DIR /Library/Vulkan/macOS)
# set(MY_LIBRARY_DIR C:/VulkanSDK/1.3.296.0)

# 设置C++ 17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# third library
set(SDL_INCLUDE_DIR ${MY_LIBRARY_DIR}/include/SDL2)
set(SDL_INCLUDE_LIB ${MY_LIBRARY_DIR}/lib)

include_directories(${SDL_INCLUDE_DIR})
link_directories(${SDL_INCLUDE_LIB})

link_libraries(SDL2-2.0.0)
# link_libraries(SDL2)

# library
include_directories(${CMAKE_SOURCE_DIR}/qbRayTrace/*)
include_directories(${CMAKE_SOURCE_DIR}/qbRayTrace/qbLinAlg)
link_directories(${SDL_LIB_DIR})

set(SOURCE_FILES
    main.cpp
    CApp.cpp
    qbRayTrace/qbImage.cpp
    qbRayTrace/scene.cpp
    qbRayTrace/ray.cpp
    qbRayTrace/camera.cpp
    qbRayTrace/qbPrimatives/objectbase.cpp
    qbRayTrace/qbPrimatives/objsphere.cpp
    qbRayTrace/qbLights/lightbase.cpp
    qbRayTrace/qbLights/pointlight.cpp
    qbRayTrace/gtfm.cpp
    qbRayTrace/qbPrimatives/objplane.cpp
    qbRayTrace/qbPrimatives/cylinder.cpp
    qbRayTrace/qbPrimatives/cone.cpp
    qbRayTrace/qbMaterials/materialbase.cpp
    qbRayTrace/qbMaterials/simplematerial.cpp
    qbRayTrace/qbTextures/texturebase.cpp
    qbRayTrace/qbTextures/flat.cpp
    qbRayTrace/qbTextures/checker.cpp
)

add_executable(RT ${SOURCE_FILES})

set_target_properties(RT PROPERTIES
    INSTALL_RPATH ${MY_LIBRARY_DIR}/lib
)