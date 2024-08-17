cmake_minimum_required(VERSION 3.22)

project(lwrb)
add_library(lwrb INTERFACE)

# Enable CMake support for C languages
enable_language(C)

file(GLOB_RECURSE lwrb_SOURCE *.c)

target_include_directories(lwrb INTERFACE
    .
)

target_sources(lwrb INTERFACE
    ${lwrb_SOURCE}
)

# Validate that lwrb code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


