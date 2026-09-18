//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_ALGORYTHM_H
#define CONTAINERS_ALGORYTHM_H

#include "iterator.h"
#include <cstdlib>
#include <ctime>

namespace mct {

    template <typename T>
    struct less{
        bool operator()(const T& a, const T& b) const {
            return a < b;
        }
    };

    template <typename T>
    void swap(T& a, T& b) {
        T c = a; a = b; b = c;
    }

    template<template<typename> class C, typename T>
    void sort(mct::iterator<C, T>& begin, mct::iterator<C, T>& end) {
        using miter = mct::iterator<C, T>;
        if (begin == end) { return; }
        miter p = partition(begin, end, comp);
    }
}

#endif //CONTAINERS_ALGORYTHM_H
