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
    src/main.cpp
    src/parsers.cpp
    src/stb_image_write.cpp
)

add_executable(
    ray_tracer_tests
    tests/raytracer_tests.cpp
    tests/test_main.cpp
    src/parsers.cpp
    src/stb_image_write.cpp
)

function(configure_raytracer_target target_name)
    target_include_directories(
        ${target_name}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_include_directories(
        ${target_name}
        SYSTEM PRIVATE
            ${stb_SOURCE_DIR}
    )

    target_compile_features(
        ${target_name}
        PRIVATE
            cxx_std_23
    )

    target_compile_options(
        ${target_name}
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(
            ${target_name}
            PRIVATE
                -g3
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
        )

        target_link_options(
            ${target_name}
            PRIVATE
                -fsanitize=address,undefined
        )
    endif()

    if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        target_compile_options(
            ${target_name}
            PRIVATE
                -fno-omit-frame-pointer
        )
    endif()
endfunction()

configure_raytracer_target(ray_tracer)
configure_raytracer_target(ray_tracer_tests)

target_link_libraries(
    ray_tracer
    PRIVATE
        nlohmann_json::nlohmann_json
)

target_link_libraries(
    ray_tracer_tests
    PRIVATE
        doctest::doctest
        nlohmann_json::nlohmann_json
)