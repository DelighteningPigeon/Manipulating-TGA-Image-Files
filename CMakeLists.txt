cmake_minimum_required(VERSION 3.10)

project(image_tool)

add_executable(tga_reader main.c)

set_target_properties(
        tga_reader
        PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

if (MSVC)
    add_compile_options(/Wall /WX)
else()
    add_compile_options(-Wall -Wextra)
endif()

