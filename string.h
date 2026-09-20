//
// Created by kirillr on 19.09.2026.
//

#ifndef CONTAINERS_STRING_H
#define CONTAINERS_STRING_H
#include <cstring>

#include "vector.h"
#include "algorythm.h"

namespace mct {
    class string: public vector<char> {
        public:
        string(): vector<char>(1, '\0') {}
        explicit string(const int64_t capacity) {
            _size = 1, _capacity = cap_count(capacity);
            _arr = static_cast<char*>(calloc(_capacity, sizeof(char)));
            if (_arr == nullptr) {throw mct::bad_alloc(_capacity, __func__);}
        }
        string(const int64_t capacity, char c) = delete;
        string(string&& s) noexcept = default;
        string(const string& s) {
            _size = s._size; _capacity = s._capacity;
            _arr = static_cast<char*>(calloc(_capacity, sizeof(char)));
            if (_arr == nullptr) {throw mct::bad_alloc(_capacity, __func__);}
            strcpy(_arr, s._arr);
        }
        string(const char* s) {
            _size = strlen(s)+1; _capacity = cap_count(_size);
            _arr = static_cast<char*>(calloc(_capacity, sizeof(char)));
            if (_arr == nullptr) {throw mct::bad_alloc(_capacity, __func__);}
            strcpy(_arr, s);
        }
        string& operator=(const string& s) {
            free(_arr); _size = s._size; _capacity = s._capacity;
            _arr = static_cast<char*>(calloc(_capacity, sizeof(char)));
            if (_arr == nullptr) {throw mct::bad_alloc(_capacity, __func__);}
            strcpy(_arr, s._arr);
        }
        string& operator=(string&& s) = default;
        string& operator=(const char* s) {
            _size = strlen(s)+1; _capacity = cap_count(_size); free(_arr);
            _arr = static_cast<char*>(calloc(_capacity, sizeof(char)));
            if (_arr == nullptr) {throw mct::bad_alloc(_capacity, __func__);}
            strcpy(_arr, s);
            return *this;
        }
        ~string() = default;
        int64_t capacity() const { return _capacity-1; }
        int64_t size() const { return _size-1; }
        int64_t len() const { return _size-1; }

        void reserve(const int64_t capacity) {
            const int64_t new_cap = cap_count(capacity);
            const auto tmp = static_cast<char*>(calloc(new_cap, sizeof(char)));
            if (tmp == nullptr) {throw mct::bad_alloc(new_cap, __func__);}
            _capacity = new_cap;
            if (_arr != nullptr) {
                strcpy(tmp, _arr); free(_arr);
            }
            _arr = tmp;
        }

        void push_back(char&& c) {
            if (_capacity == _size+1) {
                reserve(_capacity);
            }
            _arr[_size-1] = c;
        }
        void push_back(const char& c) {
            if (_capacity == _size+1) {
                reserve(_capacity);
            }
            _arr[_size++] = c;
        }
        void pop_back() {
            if (_size > 1) {
                --_size; _arr[_size-1] = '\0';
            }
        }

        const char* c_str() const { return _arr; }
        friend std::ostream& operator<<(std::ostream& os, const string& s) { os << static_cast<const char*>(s._arr); return os; }
        friend std::istream& operator>>(std::istream& is, string& s) {
            char c; is >> std::ws;
            while ( is >> c ) {
                switch ( c ) {
                    case '\n': case '\r': case '\t': case ' ': case '\0': break;
                    default: s.push_back(c);
                }
            }
            s.push_back('\n');
            return is;
        }

        friend string operator+(const string& s1, const string& s2) {
            string s; s.reserve(s1._size + s2._size-1);
            strcpy(s._arr, s1._arr);
            strcpy(s._arr + s1._size, s._arr + s2._size);
            s._size = s1._size+s2._size+1;
            return s;
        }

        friend string operator+(const char* s1, const string& s2) {
            string s; const int64_t s1_size = strlen(s1);
            s.reserve( s1_size + s2._size);
            strcpy(s._arr, s1); strcpy(s._arr + s1_size, s2._arr);
            s._size = s1_size+s2._size;
            return s;
        }

        friend string operator+(const string& s1, const char* s2) {
            string s; const int64_t s2_size = strlen(s2);
            s.reserve( s2_size + s1._size);
            strcpy(s._arr, s1._arr); strcpy(s._arr + s1._size, s2);
            s._size = s1._size+s2_size;
            return s;
        }

        friend string operator+(const string& s1, char c) {
            string s(s1._size); s.push_back(c); return s;
        }

        friend string operator+(char c, const string& s2) {
            string s(s2._size); s._arr[0] = c; strcpy(s._arr+1, s2._arr); return s;
        }

        string& operator+=(const string& s) {
            reserve(_size + s._size - 1);
            strcpy(_arr + _size, s._arr);
            return *this;
        }

        string& operator+=(const char* s) {
            reserve(_size + strlen(s));
            strcpy(_arr + _size, s);
            return *this;
        }

        string& operator+=(char c) {
            push_back(c);
            return *this;
        }

        friend int strcmp(const string& s1, const string& s2) {
            return strcmp(s1._arr, s2._arr);
        }
        friend int strcmp(const string& s1, const char* s2) {
            return strcmp(s1._arr, s2);
        }
        friend int strcmp(const char* s1, const string& s2) {
            return strcmp(s1, s2._arr);
        }
        friend string to_string(int64_t i) {
            string s; do { s.push_back(static_cast<char>('0'+i%10)); } while (i%=10);
            mct::reverse(s.begin(), s.end()-1);
            return s;
        }
    };
}

#endif //CONTAINERS_STRING_H
