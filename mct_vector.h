//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_VECTOR_H
#define CONTAINERS_VECTOR_H
#include <cstdlib>

#include "mct_move.h"
#include "mct_exception.h"
#include "mct_iterator.h"

namespace mct {
    template<class P>
        static void create(P* start, P* end) {
        while (start < end) {
            new (start) P(); ++start;
        }
    }

    template<pointer_t P>
    static void create(P* start, P* end) {
        while (start < end) {
            *start = nullptr; ++start;
        }
    }

    template<primitive_t P>
    static void create(P* start, P* end) {
        while (start < end) {
            *start = 0; ++start;
        }
    }

    template<class P>
        static void create(P* start, P* end, const P& value) {
        while (start < end) {
            new (start) P(value); ++start;
        }
    }

    template<pointer_t P>
    static void create(P* start, P* end, const P& value) {
        while (start < end) {
            *start = value; ++start;
        }
    }

    template<primitive_t P>
    static void create(P* start, P* end, const P& value) {
        while (start < end) {
            *start = value; ++start;
        }
    }

    template<class P>
        static void copy(P* start1, P* end1, P* start2, P* end2) {
        while (start1 < end1 && start2 < end2) {
            new (start1) P(*start2); ++start1; ++start2;
        }
    }

    template<pointer_t P>
    static void copy(P* start1, P* end1, P* start2, P* end2) {
        while (start1 < end1 && start2 < end2) {
            *start1 = *start2; ++start1; ++start2;
        }
    }

    template<primitive_t P>
    static void copy(P* start1, P* end1, P* start2, P* end2) {
        while (start1 < end1 && start2 < end2) {
            *start1 = *start2; ++start1; ++start2;
        }
    }

    template<class P>
    static void move_construct(P* start1, P* end1, P* start2, P* end2) {
        while (start1 < end1 && start2 < end2) {
            new(start1) P(mct::move(*start2)); ++start1; ++start2;
        }
    }

    template<pointer_t P>
    static void move_construct(P* start1, P* end1, P* start2, P* end2) {
        while (start1 < end1 && start2 < end2) {
            *start1 = *start2; ++start1; ++start2;
        }
    }

    template<primitive_t P>
    static void move_construct(P* start1, P* end1, P* start2, P* end2) {
        while (start1 < end1 && start2 < end2) {
            *start1 = *start2; ++start1; ++start2;
        }
    }

    template<class P>
    static void move_assign(P* start1, P* end1, P* start2, P* end2) {
        while (start1 < end1 && start2 < end2) {
            *start1 = mct::move(*start2); ++start1; ++start2;
        }
    }

    template<pointer_t P>
    static void move_assign(P* start1, P* end1, P* start2, P* end2) {
        while (start1 < end1 && start2 < end2) {
            *start1 = *start2; ++start1; ++start2;
        }
    }

    template<primitive_t P>
    static void move_assign(P* start1, P* end1, P* start2, P* end2) {
        while (start1 < end1 && start2 < end2) {
            *start1 = *start2; ++start1; ++start2;
        }
    }

    template<class P>
    static void destroy(P* start, P* end) {
        while (start < end) {
            start->~P(); ++start;
        }
    }
    template<pointer_t P>
    static void destroy(P* start, P* end) {
        while (start < end) {
            *start = nullptr; ++start;
        }
    }
    template<primitive_t P>
    static void destroy(P* start, P* end) {
        while (start < end) {
            *start = 0; ++start;
        }
    }

    template<typename X, typename Y>
    struct pair {
        X x; Y y;
        bool operator==(const pair& p) const {
            return x == p.x && y == p.y;
        }
        bool operator!=(const pair& p) const {
            return x != p.x || y != p.y;
        }
    };

    template<typename T>
    class vector {
    protected:
        int64_t _capacity;
        int64_t _size;
        T* _arr;

        static int64_t cap_count(int64_t new_cap, const int64_t old_cap) {
            new_cap = new_cap ? new_cap : 2;
            while (new_cap <= old_cap) { new_cap<<= 1; }
            return new_cap;
        }

    public:

        [[nodiscard]] int64_t size() const { return _size; }
        [[nodiscard]] int64_t capacity() const { return _capacity; }

        vector(): _capacity(0), _size(0), _arr(nullptr) {}

        explicit vector(const int64_t size) :
        _capacity(cap_count(size, 0)),
        _size(size),
        _arr(static_cast<T*>(malloc(sizeof(T) * _capacity))) {
            if (_arr == nullptr) { throw mct::bad_alloc(_capacity, __func__);}
            create(_arr, _arr+_size);
        }

        explicit vector(const int64_t size, const T& value) :
        _capacity(cap_count(size, 0)),
        _size(size),
        _arr(static_cast<T*>(malloc(sizeof(T) * _capacity))) {
            if (_arr == nullptr) { throw mct::bad_alloc(_capacity, __func__);}
            create(_arr, _arr+_size, value);
        }

        vector(const vector& v):
        _capacity(v._capacity),
        _size(v._size),
        _arr(static_cast<T*>(malloc(sizeof(T) * _capacity))) {
            if (_arr == nullptr) { throw mct::bad_alloc(_capacity, __func__);}
            copy(_arr, _arr+_size, v._arr, v._arr+v._size);
        }

        vector(vector&& v) noexcept :
        _capacity(v._capacity),
        _size(v._size),
        _arr(v._arr) {
            v._arr = nullptr;
            v._capacity = 0;
            v._size = 0;
        }

        template<typename I>
        requires iterator<I> && requires(I it)
        {
            requires is_same_v<decltype(*it), T&> || is_same_v<decltype(*it), const T&>;
        }
        vector(I begin, I end): vector() {
            while (begin!=end) push_back(*begin++);
        }

        ~vector() {
            destroy(_arr, _arr + _size);
            _capacity = 0;
            _size = 0;
            free(_arr);
            _arr = nullptr;
        }

        void clear() {
            destroy(_arr, _arr + _size);
            _size = 0;
        }

        vector& operator=(const vector& v) {
            if (this != &v) {
                auto tmp = static_cast<T*>(malloc(sizeof(T) * v._capacity));
                if (tmp == nullptr) {throw mct::bad_alloc(_capacity, __func__);}
                destroy(_arr, _arr + _size); free(_arr); _arr = tmp;
                copy(_arr, _arr+v._size, v._arr, v._arr+v._size);
                _capacity = v._capacity;
                _size = v._size;
            }
            return *this;
        }

        vector& operator=(vector&& v) noexcept {
            if (this != &v) {
                destroy(_arr, _arr + _size); free(_arr);
                _capacity = v._capacity;
                _size = v._size;
                _arr = v._arr;
                v._arr = nullptr;
                v._capacity = 0;
                v._size = 0;
            }
            return *this;
        }

        using iterator = T*;
        using const_iterator = const T*;

        iterator begin() { return _arr; }
        iterator end() { return _arr+_size; }
        const_iterator begin() const { return _arr; }
        const_iterator end() const { return _arr+_size; }

        void reserve(const int64_t capacity) {
            const int64_t new_cap = cap_count(capacity, _capacity);
            auto tmp = static_cast<T*>(malloc(sizeof(T) * new_cap));
            if (tmp == nullptr) {throw mct::bad_alloc(new_cap, __func__);}
            move_construct(tmp, tmp+_size, _arr, _arr+_size); free(_arr); _capacity = new_cap;
            _arr = tmp;
        }

        void resize(const int64_t size) {
            if (size < 0) throw mct::bad_index(_size, size);
            if (size > _size) {
                reserve(size);
                create(_arr+_size, _arr+size);
                _size = size;
            }
            else {
                destroy(_arr+size, _arr+_size);
                _size = size;
            }
        }

        void resize(const int64_t size, const T& value) {
            if (size < _size) throw mct::bad_index(_size, size);
            reserve(size);
            create(_arr+_size, _arr+size, value);
            _size = size;
        }

        void push_back(const T& x) {
            if (_capacity == _size) {
                reserve(_capacity);
            }
            new(_arr+_size) T(x); ++_size;
        }

        void push_back(T&& x) {
            if (_capacity == _size) {
                reserve(_capacity);
            }
            new(_arr+_size) T(mct::move(x)); ++_size;
        }

        template<typename... Args>
        void emplace_back(Args&&... args) {
            if (_capacity == _size) {
                reserve(_capacity);
            }
            new(_arr+_size) T(mct::forward<Args>(args)...); ++_size;
        }

        void pop_back() {
            if (_size > 0) {
                _arr[_size-1].~T();
                --_size;
            }
        }

        void remove(const int64_t index) {
            if (index < _size && index >= 0) {
                move_assign(_arr+index, _arr+_size-1, _arr+index+1, _arr+_size);
                --_size; _arr[_size].~T();
            }
            else throw mct::bad_index(_size, index);
        }

        T& operator[](const int64_t index) {
            if (index < _size && index >= 0) {
                return _arr[index];
            }
            throw mct::bad_index(_size, index);
        }

        const T& operator[](const int64_t index) const {
            if (index < _size && index >= 0) {
                return _arr[index];
            }
            throw mct::bad_index(_size, index);
        }
    };
}

#endif //CONTAINERS_VECTOR_H
