cmake_minimum_required(VERSION 3.22)

project(Botanica)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED 17)

# Libs
add_subdirectory(vendor/spdlog)

# Application
add_subdirectory(Application)
