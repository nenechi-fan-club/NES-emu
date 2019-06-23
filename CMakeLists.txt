cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra)

project(NES-emu)

include_directories(include)

add_subdirectory(src/nes)
add_subdirectory(src/utils)