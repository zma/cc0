cmake_minimum_required(VERSION 2.8)

add_library(flat FlatFileWriter.cpp)

add_dependencies(flat core)
