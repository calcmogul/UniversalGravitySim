cmake_minimum_required(VERSION 3.28)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(UniversalGravitySim)

include(FetchContent)

file(GLOB_RECURSE cpp_src src/*.cpp)
add_executable(UniversalGravitySim ${cpp_src})

if(NOT MSVC)
    target_compile_options(UniversalGravitySim PRIVATE -Wall -Wextra -pedantic)
else()
    # Suppress the following warnings:
    #   * C4244: lossy conversion
    target_compile_options(UniversalGravitySim PRIVATE /wd4244)
endif()
set_property(TARGET UniversalGravitySim PROPERTY COMPILE_WARNING_AS_ERROR ON)

target_compile_features(UniversalGravitySim PUBLIC cxx_std_23)
target_include_directories(UniversalGravitySim PRIVATE src)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)
target_link_libraries(UniversalGravitySim PUBLIC SFML::Graphics)

FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v2.4.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(box2d)
target_link_libraries(UniversalGravitySim PUBLIC box2d)

install(TARGETS UniversalGravitySim DESTINATION bin)
install(DIRECTORY resources DESTINATION bin)
