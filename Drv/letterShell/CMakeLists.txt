cmake_minimum_required(VERSION 3.22)

project(letterShell)
add_library(letterShell INTERFACE)

# Enable CMake support for C languages
enable_language(C)

file(GLOB_RECURSE letterShell_SOURCE *.c)

target_include_directories(letterShell INTERFACE
    .
    shellConfig
)

target_sources(letterShell INTERFACE
    ${letterShell_SOURCE}
)

# Validate that letterShell code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


