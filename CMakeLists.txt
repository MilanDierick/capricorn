cmake_minimum_required(VERSION 3.24)
project(capricorn)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog REQUIRED)
find_package(Vulkan REQUIRED)

add_library(glfw::glfw ALIAS glfw)

file(GLOB_RECURSE CAPRICORN_SOURCES
        "include/*.cpp"
        "include/*.hpp"
        "src/*.cpp"
        "src/*.hpp"
        )

add_executable(capricorn ${CAPRICORN_SOURCES})

target_include_directories(capricorn
        PRIVATE
        include
        src
        )

target_precompile_headers(capricorn
        PRIVATE
        src/capricorn/ccpch.cpp
        include/capricorn/ccpch.hpp
        )

target_compile_definitions(capricorn
        PRIVATE
        GLFW_INCLUDE_VULKAN
        )

target_link_libraries(capricorn
        PRIVATE
        glfw::glfw
        glm::glm
        spdlog::spdlog
        Vulkan::Vulkan
        )

add_custom_command(TARGET capricorn
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:capricorn>/assets)
