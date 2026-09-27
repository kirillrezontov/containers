cmake_minimum_required(VERSION 4.3)
project(containers)

set(CMAKE_CXX_STANDARD 20)

add_library(containers STATIC mct.cpp
        mct.h
        mct_vector.h
        mct_exception.h
        mct_move.h
        mct_unordered_set.h
        mct_algorithm.h
        mct_iterator.h
        mct_string.h
        #mct_flat_set.h
)
