# See how to install sdl2: https://wiki.libsdl.org/SDL2/Installation
cmake_minimum_required (VERSION 3.8)

# project name
project(LGFX_SDL LANGUAGES CXX C)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-DLGFX_SDL)

# Collect LGFX core files and user files
# NOTE: CONFIGURE_DEPENDS limit the build system dependance, we need to add all used source file here!
file(GLOB LGFX_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    src/*.cpp
    library/LovyanGFX/src/lgfx/Fonts/efont/*.c
    library/LovyanGFX/src/lgfx/Fonts/IPA/*.c
    library/LovyanGFX/src/lgfx/utility/*.c
    library/LovyanGFX/src/lgfx/v1/*.cpp
    library/LovyanGFX/src/lgfx/v1/misc/*.cpp
    library/LovyanGFX/src/lgfx/v1/panel/Panel_Device.cpp
    library/LovyanGFX/src/lgfx/v1/panel/Panel_FrameBufferBase.cpp
    library/LovyanGFX/src/lgfx/v1/platforms/sdl/*.cpp
    )

add_executable(${PROJECT_NAME} ${LGFX_SRCS})
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/library/LovyanGFX/src
    )

find_package(SDL2 REQUIRED SDL2)
if (SDL2_FOUND)
    message(STATUS "Packaged version of SDL2 will be used.")
else()
    message(STATUS "Bundled version of SDL2 will be downloaded and used.")
    include(FetchContent)
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
        GIT_TAG         5773c347d58d07a45b33ac4ecc93fd3e6a86d483
    )
    FetchContent_MakeAvailable(SDL2)
    set(SDL2_LIBRARIES ${SDL2_LIBRARIES} SDL2main SDL2-static SDL2_image-static)
endif()
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} -lpthread ${SDL2_LIBRARIES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)