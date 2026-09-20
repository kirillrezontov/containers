//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_VECTOR_H
#define CONTAINERS_VECTOR_H
#include <cstdlib>
#include "move.h"
#include "exception.h"

namespace mct {
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

        int64_t cap_count(int64_t n) const {
            n = n ? n : 2;
            while (n < _capacity) { n<<= 1; }
            return n;
        }

        static void destroy(T* start, T* end) {
            while (start < end) {
                start->~T(); ++start;
            }
        }
    public:

        int64_t size() const { return _size; }
        int64_t capacity() const { return _capacity; }

        vector(): _capacity(0), _size(0), _arr(nullptr) {}

        explicit vector(const int64_t size) :
        _capacity(cap_count(size)),
        _size(size),
        _arr(static_cast<T*>(malloc(sizeof(T) * _capacity))) {
            for (int64_t i = 0; i < _size; ++i) {
                new (_arr + i) T();
            }
        }

        explicit vector(const int64_t size, const T& value) :
        _capacity(cap_count(size)),
        _size(size),
        _arr(static_cast<T*>(malloc(sizeof(T) * _capacity))) {
            for (int64_t i = 0; i < _size; ++i) {
                new (_arr + i) T(value);
            }
        }

        vector(const vector& v):
        _capacity(v._capacity),
        _size(v._size),
        _arr(static_cast<T*>(malloc(sizeof(T) * _capacity))) {
            for (int64_t i = 0; i < _size; ++i) {
                new (_arr + i) T(v._arr[i]);
            }
        }

        vector(vector&& v) noexcept :
        _capacity(v._capacity),
        _size(v._size),
        _arr(v._arr) {
            v._arr = nullptr;
            v._capacity = 0;
            v._size = 0;
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
                destroy(_arr, _arr + _size);
                free(_arr);
                _arr = static_cast<T*>(malloc(sizeof(T) * v._capacity));
                if (_arr == nullptr) {throw mct::bad_alloc(_capacity, __func__);}
                _capacity = v._capacity;
                _size = v._size;
                for (int64_t i = 0; i < _size; ++i) {
                    new (_arr + i) T(v._arr[i]);
                }
            }
            return *this;
        }

        vector& operator=(vector&& v) noexcept {
            if (this != &v) {
                destroy(_arr, _arr + _size);
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
            int64_t new_cap = cap_count(capacity);
            auto tmp = static_cast<T*>(malloc(sizeof(T) * new_cap));
            if (tmp == nullptr) {throw mct::bad_alloc(new_cap, __func__);}
            _capacity = new_cap;
            for (int64_t i = 0; i < _size; ++i) {
                new (tmp + i) T(mct::move(_arr[i]));
            }
            free(_arr);
            _arr = tmp;
        }

        void resize(const int64_t size) {
            reserve(size);
            for (int64_t i = _size; i < size; ++i) {
                new (_arr + i) T();
            }
        }

        void resize(const int64_t size, const T& value) {
            reserve(size);
            for (int64_t i = _size; i < size; ++i) {
                new (_arr + i) T(value);
            }
        }

        void push_back(const T& x) {
            if (_capacity == _size) {
                reserve(_capacity);
            }
            new(_arr[_size]) T(x);
        }

        void push_back(T&& x) {
            if (_capacity == _size) {
                reserve(_capacity);
            }
            new(_arr[_size]) T(mct::move(x));
        }

        template<typename... Args>
        void emplace_back(Args&&... args) {
            if (_capacity == _size) {
                reserve(_capacity);
            }
            new(_arr[_size]) T(mct::forward<Args...>(args...));
        }

        void pop_back() {
            if (_size > 0) {
                --_size;
                _arr[_size].~T();
            }
        }

        void remove(const int64_t index) {
            if (index < _size && index >= 0) {
                _arr[index].~T();
                for (int64_t i = index; i < _size - 1; ++i) {
                    new (_arr + i) T(mct::move(_arr[i + 1]));
                }
                --_size;
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
