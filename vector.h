//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_VECTOR_H
#define CONTAINERS_VECTOR_H
#include <cstdlib>
#include "move.h"
#include "exception.h"
#include "exceptions.h"

template<typename T>
class vector {
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

    using iterator = T*;
    using const_iterator = const T*;

    vector(): _capacity(0), _size(0), _arr(nullptr) {}

    template<typename... Args>
    explicit vector(const int64_t size, Args&&... args) :
    _capacity(cap_count(size)),
    _size(size),
    _arr(static_cast<T*>(malloc(sizeof(T) * _capacity))) {
        for (int64_t i = 0; i < _size; ++i) {
            new (_arr + i) T(args);
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
            if (_arr == nullptr) {throw mct::bad_alloc();}
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

    iterator begin() { return _arr; }
    iterator end() { return _arr + _size; }
    const_iterator begin() const { return _arr; }
    const_iterator end() const { return _arr + _size; }

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

    template<typename... Args>
    void resize(const int64_t size, Args&&... args) {
        reserve(size);
        for (int64_t i = _size; i < size; ++i) {
            new (_arr + i) T(args));
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

};

#endif //CONTAINERS_VECTOR_H
