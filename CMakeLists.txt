cmake_minimum_required(VERSION 3.29)
project(Project_3)

set(CMAKE_CXX_STANDARD 20)

add_executable(Project_3 P3_LibraryTestSuite/libWad/main.cpp
        P3_LibraryTestSuite/libWad/Wad.cpp
        P3_LibraryTestSuite/libWad/Wad.h
)
