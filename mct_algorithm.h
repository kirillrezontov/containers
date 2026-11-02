//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_ALGORITHM_H
#define CONTAINERS_ALGORITHM_H

#include <random>

#include "mct_iterator.h"

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

    template <comparable T>
    T max(const T& a, const T& b) {
        return (a < b) ? b : a;
    }

    template <comparable T>
    T min(const T& a, const T& b) {
        return (a < b) ? a : b;
    }

    template <comparable T>
    bool maxs(T& a, const T& b) {
        if (a < b) {a = b; return true;}
        return false;
    }

    template <comparable T>
    bool mins(T& a, const T& b) {
        if (a > b) {a = b; return true;}
        return false;
    }

    template <typename T, typename C>
    concept comparator = requires(T comp, C a, C b)
    {
        requires mct::is_bool_v<decltype(comp.operator()(a,b))>;
    };

    template <typename T>
    void swap(T& a, T& b) noexcept {
        T c(mct::move(a)); a = mct::move(b); b = mct::move(c);
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
    it partition(it begin, it end, const comp& cmp, int64_t& actions) { --end;
        if (begin == end) { return begin; }
        auto pivot = median(*begin, *end, *(begin+(end-begin)/2));
        it left = begin, right = end;
        while (true) {
            while (cmp(*left, pivot)) { ++left; }
            while (cmp(pivot, *right)) { --right; }
            if (left >= right) {
                return right;
            }
            swap(*left, *right); ++actions;
            ++left; --right;
        }
        return left;
    }

    template<typename it, typename comp = less<decltype(*mct::declval<it>())>>
    requires mct::random_access_iterator<it> &&
        comparator<comp, decltype(*mct::declval<it>())>
    int64_t sort(it begin, it end, const comp& cmp = less<decltype(*mct::declval<it>())>{}) {
        if (end - begin <= 1) { return 0; }
        int64_t actions = 0;
        it p = mct::partition(begin, end, cmp, actions);
        actions+=sort(begin, p+1, cmp);
        actions+=sort(p+1, end, cmp);
        return actions;
    }

    template<typename it>
    requires mct::iterator<it>
    void reverse(it begin, it end) {
        while (begin != end) {
            --end; if (begin == end) break;
            swap(*begin, *end);
            ++begin;
        }
    }

    inline int thread_local_rand() {
        thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution distribution(INT32_MIN, INT32_MAX);
        return distribution(generator);
    }
}

#endif //CONTAINERS_ALGORITHM_H
