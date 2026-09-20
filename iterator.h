//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_ITERATOR_H
#define CONTAINERS_ITERATOR_H

#include "move.h"
#include <cstdlib>

namespace mct {
    template <typename T>
    concept iterator = requires(T it)
    {
        requires mct::is_lvalue_reference_v<decltype(*it)>;
        ++it; it++;
        it == it; it != it;
    };

    template <typename T>
    concept forward_iterator = iterator<T> && requires(T it)
    {
        T{it};
    };

    template<typename T, typename V>
    concept output_iterator = iterator<T> && requires(T it, V value)
    {
        *it = value;
    };

    template <typename T>
    concept bidirectional_iterator =
        forward_iterator<T> && requires(T it)
    {
        --it; it--;
    };

    template <typename T>
    concept random_access_iterator = bidirectional_iterator<T> && requires(T a, T b, int64_t size) {
        a + size; a - size;
        requires mct::is_lvalue_reference_v<decltype(a[size])>; a + b;
        a < b; a > b;
    };

    template <typename T>
    concept contiguous_iterator = random_access_iterator<T> && requires(T it)
    {
        requires mct::is_ptr_v<T>;
    };
}

#endif //CONTAINERS_ITERATOR_H
