cmake_minimum_required(VERSION 3.20.0)
project(long-division)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND SRCFILES tests.cpp)

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

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        182c910b4b63ff587a3440e08f84f70497e49a81 # v2.13.10
)

FetchContent_MakeAvailable(Catch2)

target_include_directories(long-division PRIVATE ${Catch2_SOURCE_DIR}/single_include)
