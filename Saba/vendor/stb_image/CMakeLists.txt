cmake_minimum_required(VERSION 3.1...3.27 FATAL_ERROR)
project(stb_image)

add_library(stb_image)
target_sources(stb_image PRIVATE stb_image.cpp)
