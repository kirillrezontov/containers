//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_UNORDERED_SET_H
#define CONTAINERS_UNORDERED_SET_H
#include <climits>
#include <cmath>
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

        switch (len & 7) {
            case 7: h ^= static_cast<u_int64_t>(data2[6]) << 48;
            case 6: h ^= static_cast<u_int64_t>(data2[5]) << 40;
            case 5: h ^= static_cast<u_int64_t>(data2[4]) << 32;
            case 4: h ^= static_cast<u_int64_t>(data2[3]) << 24;
            case 3: h ^= static_cast<u_int64_t>(data2[2]) << 16;
            case 2: h ^= static_cast<u_int64_t>(data2[1]) << 8;
            case 1: h ^= static_cast<u_int64_t>(data2[0]); h *= m;
        };
        h ^= h >> r; h *= m; h ^= h >> r;
        return h%LLONG_MAX;
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
            const int64_t y = floor(x/step + 0.5);
            return MurmurHash2(reinterpret_cast<const char*>(&y), sizeof(double));
        }
        static constexpr int64_t lookup_num = 3;
        static constexpr double epsilon = 1/(1<<23);
        static constexpr double step = 2*epsilon;
        static bool equal(const double& x, const double& y) {
            return fabs(x-y) < epsilon;
        }
        static void lookup(const double& x, int64_t* out) {
            out[0] = hash(x-epsilon);
            out[1] = hash(x);
            out[2] = hash(x+epsilon);
        }
    };

    struct node_base{
        node_base* next = nullptr;
    };

    static constexpr double rehash_mul = 2.0;

    template<typename T>
    class unordered_set {
        protected:
        int64_t _size, _capacity;
        struct node: node_base {
            T _elem;
            template<typename ...Args>
            explicit node(Args ...args) : _elem(mct::forward<Args...>(args)...) {}
            node(const node& other) : _elem(other._elem) {}
            explicit node(const T& elem) : _elem(elem) {}
            explicit node(T&& elem) : _elem(move(elem)) {}
        };
        node_base _before_list;
        int64_t _before_list_pos;
        node** _buckets;

        void insert_element(node* new_node) {
            const int64_t hval = unordered_set_traits<T>::hash(new_node->elem) % _capacity;
            if (_buckets[hval]) {
                new_node->next = _buckets[hval]->next;
                _buckets[hval]->next = new_node;
            }
            else {
                if (_before_list_pos >= 0) {
                    _buckets[_before_list_pos] = new_node;
                    new_node->next = _buckets[_before_list_pos];
                }
                _buckets[hval] = static_cast<node*>(_before_list);
                _before_list.next = static_cast<node_base*>(new_node);
                _before_list_pos = hval;
            }
        }
        void rehash(int64_t capacity) {
            if (_buckets) free(_buckets);
            _capacity = capacity;
            _buckets = static_cast<node**>(calloc(_capacity, sizeof(node)));
            if (!_buckets) throw mct::bad_alloc(_capacity*sizeof(node*), __func__);
            _before_list_pos = -1;
            node* head = _before_list.next, next = nullptr;
            _before_list.next = nullptr;
            while (head) {
                next = head->next;
                insert_element(head);
                head = next;
            }
        }

        public:
        class const_iterator {
            friend class unordered_set;
            const node* _node;
            public:
            const_iterator() : _node(nullptr) {}
            explicit const_iterator(const node* node) : _node(node) {}
            const_iterator(const const_iterator& it) : _node(it._node) {}
            const_iterator& operator++() {
                if (_node) { _node = _node->next; }
                return *this;
            }
            const_iterator operator++(int) {
                const_iterator it(_node);
                if (_node) { _node = _node->next; }
                return it;
            }
            const T& operator*() const { return _node->elem; }
            const T* operator->() const { return &(operator*()); }
        };
        const_iterator begin() const { return const_iterator(_before_list.next); }
        const_iterator end() const { return const_iterator(nullptr); }
        unordered_set() : _size(0), _capacity(0), _before_list(nullptr), _before_list_pos(-1), _buckets(nullptr) {}
        unordered_set(const unordered_set& other): _size(other._size), _capacity(other._capacity), _before_list_pos(other._before_list_pos) {
            _buckets = static_cast<node**>(calloc(_capacity, sizeof(node*)));
            if (!_buckets) throw mct::bad_alloc(_capacity*sizeof(node*), __func__);
            auto* prev = &_before_list; int64_t prev_pos = -1;
            for (const auto& i : other) {
                node* nd = new node(i);
                const auto pos = unordered_set_traits<T>::hash(nd->elem) % _capacity;
                prev->next = nd; if (prev_pos != pos) { _buckets[pos] = prev;}
                prev_pos = pos; prev = nd;
            }
        }
        unordered_set(unordered_set&& other) noexcept : _size(other._size), _capacity(other._capacity), _before_list(other._before_list), _before_list_pos(other._before_list_pos),
        _buckets(other._buckets) {
            other._size = 0; other._capacity = 0; other._before_list.next = nullptr; other._buckets = nullptr; other._before_list_pos = -1;
        }
        bool insert(const T& x) {
            if (contains(x)) return false;
            if ((_size+1)<<1 >= _capacity) { rehash(rehash_mul*_capacity); }
            node* new_node = new node(x);
            insert_element(new_node); _size++;
            return true;
        }
        bool insert(T&& x) {
            if (contains(x)) return false;
            if ((_size+1)<<1 >= _capacity) { rehash(rehash_mul*_capacity); }
            node* new_node = new node(move(x));
            insert_element(new_node); _size++;
            return true;
        }
        template<typename ... Args>
        bool insert(Args&& ... args) {
            if ((_size+1)<<1 >= _capacity) { rehash(rehash_mul*_capacity); }
            node* new_node = new node(mct::forward<Args>(args)...);
            if (contains(new_node->_elem)) {delete new_node; return false;}
            insert_element(new_node); _size++;
            return true;
        }
        bool contains(const T& x) {
            int64_t hvals[unordered_set_traits<T>::lookup_num];
            unordered_set_traits<T>::lookup(x, hvals);
            for (int64_t i = 0; i < unordered_set_traits<T>::lookup_num; ++i) hvals[i]%=_capacity;
            for (int64_t i = 0; i < unordered_set_traits<T>::lookup_num; ++i) {
                for (node* it = static_cast<node*>(_before_list.next); it; it = it->next) {
                    if (unordered_set_traits<T>::equal(it->_elem, x)) {return true;}
                    if (unordered_set_traits<T>::hash(it->_elem) != hvals[i]) {break;}
                }
            }
            return false;
        }
        const_iterator find(const T& x) {
            int64_t hvals[unordered_set_traits<T>::lookup_num];
            unordered_set_traits<T>::lookup(x, hvals);
            for (int64_t i = 0; i < unordered_set_traits<T>::lookup_num; ++i) hvals[i]%=_capacity;
            for (int64_t i = 0; i < unordered_set_traits<T>::lookup_num; ++i) {
                for (auto it = const_iterator(_buckets[hvals[i]]); it!=end(); ++it) {
                    if (unordered_set_traits<T>::equal(*it, x)) {return it;}
                    if (unordered_set_traits<T>::hash(*it) != hvals[i]) {break;}
                }
            }
            return end();
        }
        bool remove(const T& x) {
            auto it = find(x);
            if (it == end()) return false;
            auto nd = it._node;
            auto this_hval = unordered_set_traits<T>::hash(static_cast<node*>(nd)->elem)%_capacity;
            node* prev = _buckets[this_hval];
            if (nd->next) {
                auto next_hval = unordered_set_traits<T>::hash(static_cast<node*>(nd->next)->elem)%_capacity;
                _buckets[next_hval] = _buckets[this_hval];
            }
            if (prev->next == nd) {
                _buckets[this_hval] = nullptr;
            }
            while (prev->next != nd) { prev = prev->next; }
            prev->next = nd->next;
            delete nd;
            return true;
        }
        bool remove(const_iterator& it) {
            if (it == end()) return false;
            auto nd = it._node;
            auto this_hval = unordered_set_traits<T>::hash(static_cast<node*>(nd)->elem)%_capacity;
            node* prev = _buckets[this_hval];
            if (nd->next) {
                auto next_hval = unordered_set_traits<T>::hash(static_cast<node*>(nd->next)->elem)%_capacity;
                _buckets[next_hval] = _buckets[this_hval];
            }
            if (prev->next == nd) {
                _buckets[this_hval] = nullptr;
            }
            while (prev->next != nd) { prev = prev->next; }
            prev->next = nd->next;
            ++it; delete nd;
            return true;
        }
    };
} // mcp

#endif //CONTAINERS_SET_H
