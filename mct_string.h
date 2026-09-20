//
// Created by kirillr on 19.09.2026.
//

#ifndef CONTAINERS_STRING_H
#define CONTAINERS_STRING_H
#include <cstring>

#include "mct_vector.h"
#include "mct_algorithm.h"

namespace mct {
    class bad_string : public mct::exception {
        const char* func;
        public:
        explicit bad_string(const char* func) : func(func) {}
        void what() const override { std::cerr << "bad_string " << func << std::endl; }
    };

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
            if (s._arr == nullptr) {throw mct::bad_string(__func__);}
            _size = s._size; _capacity = s._capacity;
            _arr = static_cast<char*>(calloc(_capacity, sizeof(char)));
            if (_arr == nullptr) {throw mct::bad_alloc(_capacity, __func__);}
            strcpy(_arr, s._arr);
        }
        string(const char* s) {
            if (s == nullptr) throw mct::bad_string(__func__);
            _size = strlen(s)+1; _capacity = cap_count(_size);
            _arr = static_cast<char*>(calloc(_capacity, sizeof(char)));
            if (_arr == nullptr) {throw mct::bad_alloc(_capacity, __func__);}
            strcpy(_arr, s);
        }
        string& operator=(const string& s) {
            if (s._arr == nullptr) {throw mct::bad_string(__func__);}
            auto tmp = static_cast<char*>(calloc(s._capacity, sizeof(char)));
            if (tmp == nullptr) {throw mct::bad_alloc(_capacity, __func__);}
            free(_arr); _size = s._size; _capacity = s._capacity; _arr = tmp;
            strcpy(_arr, s._arr);
            return *this;
        }
        string& operator=(string&& s) = default;
        string& operator=(const char* s) {
            if (s == nullptr) throw mct::bad_string(__func__);
            const int64_t size = strlen(s)+1, capacity = cap_count(size);
            auto tmp = static_cast<char*>(calloc(capacity, sizeof(char)));
            if (tmp == nullptr) {throw mct::bad_alloc(capacity, __func__);}
            free(_arr); _size = size; _capacity = capacity; _arr = tmp;
            strcpy(_arr, s);
            return *this;
        }
        ~string() = default;
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
        friend std::ostream& operator<<(std::ostream& os, const string& s) { os << s._arr; return os; }
        friend std::istream& operator>>(std::istream& is, string& s) {
            char c; is >> std::ws;
            while ( is >> c ) {
                switch ( c ) {
                    case '\n': case '\r': case '\t': case ' ': case '\0': break;
                    default: s.push_back(c);
                }
            }
            return is;
        }

        friend string operator+(const string& s1, const string& s2) {
            string s; s.reserve(s1._size + s2._size-1);
            strcpy(s._arr, s1._arr);
            strcpy(s._arr + s1._size - 1, s2._arr);
            s._size = s1._size+s2._size-1;
            return s;
        }

        friend string operator+(const char* s1, const string& s2) {
            if (s1 == nullptr) throw mct::bad_string(__func__);
            string s; const int64_t s1_size = strlen(s1);
            s.reserve( s1_size + s2._size);
            strcpy(s._arr, s1); strcpy(s._arr + s1_size, s2._arr);
            s._size = s1_size+s2._size;
            return s;
        }

        friend string operator+(const string& s1, const char* s2) {
            if (s2 == nullptr) throw mct::bad_string(__func__);
            string s; const int64_t s2_size = strlen(s2);
            s.reserve( s2_size + s1._size);
            strcpy(s._arr, s1._arr); strcpy(s._arr + s1._size - 1, s2);
            s._size = s1._size+s2_size;
            return s;
        }

        friend string operator+(const string& s1, const char c) {
            string s(s1._size); s.push_back(c); return s;
        }

        friend string operator+(const char c, const string& s2) {
            string s(s2._size); s._arr[0] = c; strcpy(s._arr+1, s2._arr); return s;
        }

        string& operator+=(const string& s) {
            reserve(_size + s._size - 1);
            strcpy(_arr + _size - 1, s._arr);
            return *this;
        }

        string& operator+=(const char* s) {
            if (s == nullptr) throw mct::bad_string(__func__);
            reserve(_size + strlen(s));
            strcpy(_arr + _size - 1, s);
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
            if (s2 == nullptr) throw mct::bad_string(__func__);
            return strcmp(s1._arr, s2);
        }
        friend int strcmp(const char* s1, const string& s2) {
            if (s1 == nullptr) throw mct::bad_string(__func__);
            return strcmp(s1, s2._arr);
        }
        friend string to_string(int64_t i) {
            char sign = (i < 0) ? -1 : 1;
            i = (i < 0) ? -i : i;
            string s; do { s.push_back(static_cast<char>('0'+i%10)); } while (i%=10);
            if (sign) s.push_back('-');
            mct::reverse(s.begin(), s.end()-1);
            return s;
        }
    };
}

#endif //CONTAINERS_STRING_H
