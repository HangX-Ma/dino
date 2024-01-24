# See how to install sdl2: https://wiki.libsdl.org/SDL2/Installation
cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc-posix)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++-posix)
message(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

option(BUILD_STATIC "build output executable using static library")
if(BUILD_STATIC)
    message(STATUS "Build with static library")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# project name
project(dino LANGUAGES CXX C)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-s -Os -ffunction-sections -fdata-sections -Wl,--gc-section)
    add_compile_options(-fsanitize-address-outline-instrumentation)
else()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(
        -Wall # print warning info
        -Wextra # print extra warning info
    )
    add_compile_options(-Og -g)
endif()

add_definitions(-DLGFX_SDL)

# Collect LGFX core files and user files
# NOTE: CONFIGURE_DEPENDS limit the build system dependance, we need to add all used source file here!
file(GLOB LGFX_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    simulator/*.cpp
    library/LovyanGFX/src/lgfx/Fonts/efont/*.c
    library/LovyanGFX/src/lgfx/Fonts/IPA/*.c
    library/LovyanGFX/src/lgfx/utility/*.c
    library/LovyanGFX/src/lgfx/v1/*.cpp
    library/LovyanGFX/src/lgfx/v1/misc/*.cpp
    library/LovyanGFX/src/lgfx/v1/panel/Panel_Device.cpp
    library/LovyanGFX/src/lgfx/v1/panel/Panel_FrameBufferBase.cpp
    library/LovyanGFX/src/lgfx/v1/platforms/sdl/*.cpp
    )
file(GLOB DINO_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/include/dino/*.cpp)

add_executable(${PROJECT_NAME} ${LGFX_SRCS} ${DINO_SRCS})
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/simulator
    ${CMAKE_CURRENT_SOURCE_DIR}/library/LovyanGFX/src)

set(SDL2_LIB_PATH "$ENV{HOME}/SDL2-2.28.5/x86_64-w64-mingw32/lib")
target_include_directories(${PROJECT_NAME} PUBLIC "$ENV{HOME}/SDL2-2.28.5/x86_64-w64-mingw32/include" library/mingw_stdthreads)
target_link_libraries(${PROJECT_NAME} PUBLIC "${SDL2_LIB_PATH}/libSDL2.dll.a")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)