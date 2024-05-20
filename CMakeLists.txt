cmake_minimum_required(VERSION 3.0.0)
project(long-division)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND SRCFILES long-division.cpp)

add_executable(long-division ${SRCFILES})

set_target_properties(long-division PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if (MSVC)
    target_compile_options(long-division PRIVATE /W4)
else()
    target_compile_options(long-division PRIVATE -Wall)
endif()