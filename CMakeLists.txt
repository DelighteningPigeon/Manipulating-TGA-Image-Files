cmake_minimum_required(VERSION 3.10)

project(Manipulating-TGA-Files)

add_executable(Manipulating-TGA-Files main.c)

set_target_properties(
        Manipulating-TGA-Files
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

