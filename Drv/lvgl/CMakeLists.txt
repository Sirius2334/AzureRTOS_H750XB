cmake_minimum_required(VERSION 3.22)

project(lvgl)
add_library(lvgl INTERFACE)

# Enable CMake support for C languages
enable_language(C)

file(GLOB_RECURSE lvgl_SOURCE *.c)

# Add project symbols (macros)
target_compile_definitions(lvgl INTERFACE
    # Add user defined symbols
    LV_LVGL_H_INCLUDE_SIMPLE
)

target_include_directories(lvgl INTERFACE
    .
)

target_sources(lvgl INTERFACE
    ${lvgl_SOURCE}
)

# Validate that lvgl code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


