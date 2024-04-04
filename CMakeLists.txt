cmake_minimum_required(VERSION 3.26.4)
project(cppproj)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${CMAKE_PROJECT_NAME} source/main.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE header)
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE library)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -std=c++20 -O2 -Werror -Wall -Wextra -pedantic -Wfloat-equal -Wundef
    -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5
    -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum
    -Wconversion -Wunreachable-code
)
