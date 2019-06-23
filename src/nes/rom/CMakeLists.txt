cmake_minimum_required (VERSION 3.8)

project(rom)

add_library(rom rom.cc)

target_link_libraries(rom)