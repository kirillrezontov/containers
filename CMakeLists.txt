cmake_minimum_required(VERSION 4.3)
project(containers)

set(CMAKE_CXX_STANDARD 20)

add_library(containers STATIC library.cpp
        vector.h
        exception.h
        library.cpp
        move.h
        set.h
        algorythm.h
        iterator.h)
