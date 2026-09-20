//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_ALGORYTHM_H
#define CONTAINERS_ALGORYTHM_H

#include "iterator.h"

namespace mct {
    template<typename T>
    concept comparable = requires(T a, T b)
    {
        requires mct::is_bool_v<decltype(a < b)>;
    };

    template <comparable T>
    struct less{
        bool operator()(const T& a, const T& b) const {
            return a < b;
        }
    };

    template <typename T, typename C>
    concept comparator = requires(T comp, C a, C b)
    {
        requires mct::is_bool_v<decltype(comp.operator()(a,b))>;
    };

    template <typename T>
    void swap(T& a, T& b) noexcept {
        T c = a; a = b; b = c;
    }

    template <comparable T>
    T median(const T& a, const T& b, const T& c) {
        if (a < b) {
            if (c < a) return a;
            if (c < b) return c;
            return b;
        }
        if (c < a) {
            if (b < c) return c;
            return b;
        }
        return a;
    }

    template<typename it, typename comp>
    requires mct::random_access_iterator<it> &&
        comparator<comp, decltype(*mct::declval<it>())>
    it partition(it begin, it end, const comp& cmp) {
        auto pivot = median(*begin, *end, *(begin+(end-begin)/2));
        it left = begin, right = end-1;
        while (true) {
            while (cmp(*left, pivot)) { ++left; }
            while (cmp(pivot, *right)) { --right; }
            if (left >= right) {
                return right;
            }
            swap(*left, *right);
            ++left; --right;
        }
        return left;
    }

    template<typename it, typename comp>
    requires mct::random_access_iterator<it> &&
        comparator<comp, decltype(*mct::declval<it>())>
    void sort(it begin, it end, const comp& cmp = less<decltype(*mct::declval<it>())>{}) {
        if (end <= begin) { return; }
        it p = partition(begin, end, cmp);
        sort(begin, p+1, cmp);
        sort(p+1, end, cmp);
    }

    template<typename it>
    requires mct::iterator<it>
    void reverse(it begin, it end) {
        --end; while (begin != end) {
            swap(*begin, *end);
            ++begin; --end;
        }
    }

}

#endif //CONTAINERS_ALGORYTHM_H
