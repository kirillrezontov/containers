//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_SET_H
#define CONTAINERS_SET_H
#include <stdint.h>

#include "vector.h"

namespace mct {

    inline int64_t MurmurHash2 (const char* key, const u_int64_t len) {
        constexpr u_int64_t m = 0xc6a4a7935bd1e995ULL, seed = 0;
        constexpr int r = 47;
        u_int64_t h = seed ^ (len * m);

        auto data = reinterpret_cast<const u_int64_t*>(key);
        const u_int64_t* end = data + (len/8);

        while (data != end)
        {
            u_int64_t k = *data++;
            k *= m; k ^= k >> r; k *= m;
            h ^= k; h *= m;
        }

        const auto data2 = reinterpret_cast<const u_char*>(data);

        switch (len & 7)
        {
            case 7:
                h ^= static_cast<u_int64_t>(data2[6]) << 48;
            case 6:
                h ^= static_cast<u_int64_t>(data2[5]) << 40;
            case 5:
                h ^= static_cast<u_int64_t>(data2[4]) << 32;
            case 4:
                h ^= static_cast<u_int64_t>(data2[3]) << 24;
            case 3:
                h ^= static_cast<u_int64_t>(data2[2]) << 16;
            case 2:
                h ^= static_cast<u_int64_t>(data2[1]) << 8;
            case 1:
                h ^= static_cast<u_int64_t>(data2[0]);
                h *= m;
        };

        h ^= h >> r; h *= m; h ^= h >> r;
        return static_cast<int64_t>(h%INT64_MAX);
    }

    template<typename T>
    struct unordered_set_traits {
        static int64_t hash(const T& x) {
            return MurmurHash2(reinterpret_cast<const char*>(&x), sizeof(T));
        }
        static bool equal(const T& x, const T& y) {
            return x == y;
        }
        static constexpr int64_t lookup_num = 1;
        static void lookup(const T& x, int64_t* out) {
            out[0] = hash(x);
        }
    };

    template<>
    struct unordered_set_traits<double> {
        static int64_t hash(const double& x) {
            return MurmurHash2(reinterpret_cast<const char*>(&x), sizeof(double));
        }
        static bool equal(const double& x, const double& y) {

        }
    }


    template<typename T>
    class unordered_set {

    };
} // mcp

#endif //CONTAINERS_SET_H
