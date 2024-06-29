
set(
    APPLICATION_SOURCES

    src/EntryPoint.cpp
    src/Application.cpp
    src/Log.cpp
)

add_executable(Application ${APPLICATION_SOURCES})

target_include_directories(
    Application PRIVATE
    src/
)

find_package(OpenGL REQUIRED)
message(STATUS "OpenGL found: ${OPENGL_FOUND}")

target_link_libraries(
    Application PRIVATE
    OpenGL::GL
    spdlog_header_only
    glfw
)

target_precompile_headers(Application PRIVATE src/btpch.h)
