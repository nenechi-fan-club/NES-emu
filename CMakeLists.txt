cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

project(NES-emu)

include_directories(include)

add_subdirectory(src/nes)
add_subdirectory(src/utils)

add_subdirectory(src/disassembler)