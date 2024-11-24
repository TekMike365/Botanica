cmake_minimum_required(VERSION 3.22)

project(Botanica VERSION 0.1)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED 17)

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
    src/btpch.h
)

set(
    INCLUDE_DIRS
    src/
)

set(
    APPLICATION_SOURCES

    src/EntryPoint.cpp
    src/Log.cpp
    src/TestLayer.cpp
    
    src/Core/Application.cpp
    src/Core/Layer.cpp
    src/Core/LayerStack.cpp
    src/Core/RenderLayer.cpp

    src/Core/Platform/OpenGL/CommandBuffer.cpp
    src/Core/Platform/OpenGL/Context.cpp
    src/Core/Platform/OpenGL/RendererAPI.cpp
    src/Core/Platform/OpenGL/Buffer.cpp
    src/Core/Platform/OpenGL/VertexArray.cpp
    src/Core/Platform/OpenGL/Shader.cpp
    src/Core/Platform/CrossPlatform/CrossPlatformWindow.cpp
    src/Core/Platform/CrossPlatform/CrossPlatformPlatform.cpp
    src/Core/Platform/CrossPlatform/CrossPlatformInput.cpp
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
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Building in Debug mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    add_compile_definitions(BT_BUILD_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Building in Release mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    add_compile_definitions(BT_BUILD_RELEASE)
endif()

add_executable(Application ${APPLICATION_SOURCES})

target_compile_definitions(Application PUBLIC PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/")

target_precompile_headers(Application PRIVATE ${PRECOMPILED_HEADERS_FILE})

target_include_directories(
    Application PRIVATE
    ${INCLUDE_DIRS}
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
