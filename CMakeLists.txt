cmake_minimum_required(VERSION 3.22)

project(Botanica VERSION 0.1)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED 17)
set(CXX_EXTENSIONS OFF)

add_subdirectory(vendor/glad)
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/spdlog)

add_compile_definitions(
    GLFW_INCLUDE_NONE
    BT_ENABLE_ASSERTS
)

set(
    PRECOMPILED_HEADERS_FILE
    src/pch.h
)

set(
    INCLUDE_DIRS
    src/
)

set(
    APPLICATION_SOURCES
    src/main.cpp
    src/Log.cpp
    src/Window.cpp
    src/Input.cpp
    src/Application.cpp
    src/LayerStack.cpp
    src/Platform.cpp
    src/pch.cpp

    src/Renderer/Renderer.cpp
    src/Renderer/Buffer.cpp
    src/Renderer/Shader.cpp
    src/Renderer/VertexArray.cpp

    src/Layers/CameraController.cpp
    src/Layers/RenderingLayer.cpp
    src/Layers/SimulationLayer.cpp

    src/Components/Plant.cpp
)

set(
    SHADER_SOURCES
    src/shaders/VoxelVert.glsl
    src/shaders/VoxelFrag.glsl
    src/shaders/WorldBoundsVert.glsl
    src/shaders/WorldBoundsFrag.glsl
)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "Building for Linux")
    add_compile_definitions(BT_PLATFORM_LINUX)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "Building for Windows")
    add_compile_definitions(BT_PLATFORM_WINDOWS)
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message(STATUS "Building for MacOS")
    add_compile_definitions(BT_PLATFORM_MACOS)
else()
    message(WARNING "Unknown system.")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Building in Debug mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    add_compile_definitions(BT_BUILD_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Building in Release mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    add_compile_definitions(BT_BUILD_RELEASE)
else()
    message(WARNING "Building in (unknown): ${CMAKE_BUILD_TYPE}")
endif()


find_package(Python3 REQUIRED COMPONENTS Interpreter)
if (NOT Python3_FOUND)
    message(FATAL_ERROR "couldn't find Python3. required")
endif()

message(STATUS "Pyhton: ${Python3_EXECUTABLE}")

set(SHADERS_HEADER src/shaders/Shader.h)
add_custom_command(
    OUTPUT ${SHADERS_HEADER}
    DEPENDS ${SHADER_SOURCES}
    COMMAND cd ${PROJECT_SOURCE_DIR} && ${Python3_EXECUTABLE} scripts/embed_shaders.py
)
add_custom_target(EmbedShaders DEPENDS ${SHADERS_HEADER})

add_executable(Application ${APPLICATION_SOURCES})

target_compile_definitions(Application PRIVATE PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/")

target_precompile_headers(Application PRIVATE ${PRECOMPILED_HEADERS_FILE})

target_include_directories(
    Application PRIVATE
    ${INCLUDE_DIRS}
)

find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
    message(FATAL_ERROR "couldn't find OpenGL. required")
endif()

target_link_libraries(
    Application PRIVATE
    OpenGL::GL
    spdlog_header_only
    glfw
    glad
    glm
)
