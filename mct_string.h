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
        string(): vector(1) {}
        string(const string& s) = default;
        explicit string(const char* s) {
            if (s == nullptr) throw mct::bad_string(__func__);
            const int64_t s_size = strlen(s);
            reserve(s_size+1); strcpy(_arr, s);
            _size = s_size+1;
        }
        explicit string(const char c): vector(2) {
            _arr[0] = c;
        }
        string& operator=(const string& s) = default;
        string& operator=(const char* s) {
            if (s == nullptr) throw mct::bad_string(__func__);
            const int64_t s_size = strlen(s);
            reserve(s_size+1); strcpy(_arr, s);
            _size = s_size+1;
            return *this;
        }
        string& operator=(const char c) {
            resize(2);
            _arr[0] = c; _arr[1] = '\0';
            return *this;
        }
        void push_back(const char c) {
            reserve(_capacity);
            _arr[_size-1] = c; _arr[_size] = '\0'; ++_size;
        }
        const char* c_str() const { return _arr; }
        friend std::ostream& operator<<(std::ostream& os, const string& s) { os << s._arr; return os; }
        friend std::istream& operator>>(std::istream& is, string& s) {
            char c; is >> std::ws;
            while ( is >> c ) {
                switch ( c ) {
                    case '\n': case '\r': case '\t': case ' ': case '\0': s.push_back('\n'); break;
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
            string s(s1); s.push_back(c); return s;
        }

        friend string operator+(const char c, const string& s2) {
            string s(s2._size+1); s._arr[0] = c; strcpy(s._arr+1, s2._arr); s._size = s2._size + 1; return s;
        }

        string& operator+=(const string& s) {
            reserve(_size + s._size - 1);
            strcpy(_arr + _size - 1, s._arr);
            _size += s._size - 1;
            return *this;
        }

        string& operator+=(const char* s) {
            if (s == nullptr) throw mct::bad_string(__func__);
            const int64_t s_size = strlen(s); reserve(_size + s_size);
            strcpy(_arr + _size - 1, s);
            _size += s_size - 1;
            return *this;
        }

        string& operator+=(const char c) {
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
        friend string to_string(int64_t i);
    };

    inline string to_string(int64_t i) {
        const char sign = (i < 0) ? 1 : 0;
        i = (i < 0) ? -i : i;
        string s; do { s.push_back(static_cast<char>('0'+i%10)); } while (i/=10);
        if (sign) s.push_back('-');
        mct::reverse(s.begin(), s.end()-1);
        return s;
    }
}

#endif //CONTAINERS_STRING_H
