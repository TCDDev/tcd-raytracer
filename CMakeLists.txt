cmake_minimum_required(VERSION 3.20)

project(ray_tracer LANGUAGES CXX)

include(FetchContent)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.5.3

)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG 2c980bb59875b0d32144a71867fbdebb2f77cd20
)

FetchContent_MakeAvailable(doctest)
FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(stb)

add_executable(
    ray_tracer
    raytracer_tests.cpp
    test_main.cpp
    src/parsers.cpp
)

target_include_directories(
    ray_tracer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(
    ray_tracer
    SYSTEM PRIVATE
        ${stb_SOURCE_DIR}   
)

target_link_libraries(
    ray_tracer
    PRIVATE
        doctest::doctest
        nlohmann_json::nlohmann_json

)

target_compile_features(
    ray_tracer
    PRIVATE
        cxx_std_23
)

target_compile_options(
    ray_tracer
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic   
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(
        ray_tracer
        PRIVATE
            -g3
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
    )

    target_link_options(
        ray_tracer
        PRIVATE
            -fsanitize=address,undefined
    )
endif()

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
target_compile_options(
    ray_tracer
    PRIVATE
        -fno-omit-frame-pointer
)
endif()

target_link_options(
    ray_tracer
    PRIVATE
        -fsanitize=address,undefined   
)