cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
project(tiff-learn)

find_package(TIFF REQUIRED)
include_directories(.)


add_executable(test src/IO.cpp)

target_link_libraries (test TIFF::TIFF)