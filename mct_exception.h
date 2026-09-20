//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_EXCEPTION_H
#define CONTAINERS_EXCEPTION_H
#include <cstdlib>
#include <iostream>
#include <ostream>

namespace mct {
    class exception {
        public:
        exception();
        virtual ~exception() = 0;
        virtual void what() const = 0;
    };

    class bad_alloc : public exception {
        int64_t sz;
        const char* func;
        public:
        bad_alloc(int64_t size, const char* func):sz(size), func(func) {}
        void what() const override { std::cerr << "bad_alloc " << func << ' ' << sz << std::endl; }
    };

    class bad_index : public exception {
        int64_t sz;
        int64_t id;
        public:
        bad_index(int64_t size, int64_t index):sz(size), id(index) {}
        void what() const override { std::cerr << "bad_index " << id << ' ' << sz << std::endl; }
    };

    class bad_file : public exception {
        const char* file;
        const char* func;
        public:
        bad_file(const char* file, const char* func):file(file), func(func) {}
        void what() const override { std::cerr << "bad_file " << file << ' ' << func << std::endl; }
    };

}

#endif //CONTAINERS_EXCEPTIONS_H
