cmake_minimum_required(VERSION 3.22)

project(segger)
add_library(segger INTERFACE)

# Enable CMake support for C languages
enable_language(C)

file(GLOB_RECURSE segger_SOURCE *.c)

target_include_directories(segger INTERFACE
    .
)

target_sources(segger INTERFACE
    ${segger_SOURCE}
)

# Validate that segger code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


