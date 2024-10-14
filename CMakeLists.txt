cmake_minimum_required(VERSION 3.22)

project(Botanica VERSION 0.1)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED 17)

# Libs
add_subdirectory(vendor/glad)
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/spdlog)

# Application
set(
    APPLICATION_SOURCES

    src/Core/EntryPoint.cpp
)

set(
    INCLUDE_DIRS
    src/
)

add_executable(Application ${APPLICATION_SOURCES})

target_compile_definitions(Application PUBLIC PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/")

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_include_directories(
    Application PRIVATE
    &{INCLUDE_DIRS}
)

find_package(OpenGL REQUIRED)
message(STATUS "OpenGL found: ${OPENGL_FOUND}")

target_link_libraries(
    Application PRIVATE
    OpenGL::GL
    spdlog_header_only
    glfw
    glad
    glm
)
