
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

target_link_libraries(
    Application PRIVATE
    spdlog_header_only
)
