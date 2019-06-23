cmake_minimum_required (VERSION 3.8)

project(utils)

add_library(utils disassembler6502.cc)

target_link_libraries(utils rom)