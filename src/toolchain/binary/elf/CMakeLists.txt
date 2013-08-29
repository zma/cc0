cmake_minimum_required(VERSION 2.8)

add_library(elf ElfFileWriter.cpp )

add_dependencies(elf core)