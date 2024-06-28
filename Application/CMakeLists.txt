
set( APPLICATION_SOURCES
    src/EntryPoint.cpp
    src/Application.cpp
)

add_executable(Application ${APPLICATION_SOURCES})

target_include_directories(Application PRIVATE
    src/
)
