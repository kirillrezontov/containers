//
// Created by kirillr on 18.09.2026.
//

#ifndef CONTAINERS_ITERATOR_H
#define CONTAINERS_ITERATOR_H

namespace mct {

    template <template <typename> class C, typename T>
    class iterator {
    protected:
        T* val;
        C<T>& ownr;
    public:
        iterator(T* value, C<T>& owner) : val(value), ownr(owner) {}
        iterator(const iterator& other) : val(other.val), ownr(other.ownr) {}
        T* operator->() { return val; }
        T& operator*() { return *val; }
        iterator& operator++() {
            ++val;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp(*this);
            ++val;
            return tmp;
        }
        iterator& operator--() {
            --val;
            return *this;
        }
        iterator operator--(int) {
            iterator tmp(*this);
            --val;
            return tmp;
        }
        bool operator==(const iterator& other) const {
            return val == other.val;
        }
        bool operator!=(const iterator& other) const {
            return val != other.val;
        }
    };


    template <template <typename> class C, typename T>
    class const_iterator {
    protected:
        const T* val;
        const C<T>& ownr;
    public:
        const_iterator(const T* value, const C<T>& owner) : val(value), ownr(owner) {}
        const_iterator(const const_iterator& other) : val(other.val), ownr(other.ownr) {}
        const_iterator(const iterator<C, T>& other) : val(other.val), ownr(other.ownr) {}
        const T* operator->() const { return val; }
        const T& operator*() const { return *val; }
        const_iterator& operator++() {
            ++val;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp(*this);
            ++val;
            return tmp;
        }
        const_iterator& operator--() {
            --val;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp(*this);
            --val;
            return tmp;
        }
        bool operator==(const const_iterator& other) const {
            return val == other.val;
        }
        bool operator!=(const const_iterator& other) const {
            return val != other.val;
        }
    };

    template <template <typename> class C, typename T>
    class reverse_iterator : public iterator<C, T> {
    public:
        reverse_iterator(T* value, C<T>& owner) : iterator<C, T>(value, owner) {}
        reverse_iterator(const reverse_iterator& other) : iterator<C, T>(other.val, other.ownr) {}
        T* operator->() { return this->val; }
        T& operator*() { return *(this->val); }
        reverse_iterator& operator++() {
            --this->val;
            return *this;
        }
        reverse_iterator operator++(int) {
            reverse_iterator tmp(*this);
            --this->val;
            return tmp;
        }
        reverse_iterator& operator--() {
            ++this->val;
            return *this;
        }
        reverse_iterator operator--(int) {
            reverse_iterator tmp(*this);
            ++this->val;
            return tmp;
        }
        bool operator==(const reverse_iterator& other) const {
            return this->val == other.val;
        }
        bool operator!=(const reverse_iterator& other) const {
            return this->val != other.val;
        }
    };

    template<template <typename> class C, typename T>
    class const_reverse_iterator : public const_iterator<C, T> {
        public:
        const_reverse_iterator(const T* value, C<T>& owner) : const_iterator<C, T>(value, owner) {}
        const_reverse_iterator(const const_reverse_iterator& other) : const_iterator<C, T>(other.val, other.ownr) {}
        const_reverse_iterator(const reverse_iterator<C, T>& other) : const_iterator<C, T>(other.val, other.ownr) {}
        const T* operator->() const { return this->val; }
        const T& operator*() const { return *(this->val); }
        const_reverse_iterator& operator++() {
            --this->val;
            return *this;
        }
        const_reverse_iterator operator++(int) {
            const_reverse_iterator tmp(*this);
            --this->val;
            return tmp;
        }
        const_reverse_iterator& operator--() {
            ++this->val;
            return *this;
        }
        const_reverse_iterator operator--(int) {
            const_reverse_iterator tmp(*this);
            ++this->val;
            return tmp;
        }
        bool operator==(const const_reverse_iterator& other) const {
            return this->val == other.val;
        }
        bool operator!=(const const_reverse_iterator& other) const {
            return this->val != other.val;
        }
    };

}

#endif //CONTAINERS_ITERATOR_H
