cmake_minimum_required (VERSION 3.8)

project(disassembler)

add_executable(disassembler main.cc)

target_link_libraries(disassembler utils rom)