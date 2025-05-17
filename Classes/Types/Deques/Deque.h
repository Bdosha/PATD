#ifndef DEQUE_H
#define DEQUE_H
#include "../../Sequences/Sequence.h"
#pragma once

template<class T>
class Deque {
public:
    Deque();

    explicit Deque(Sequence<T> *);

    Deque(const Deque &);

    Deque(T *, int);

    ~Deque();

    Deque &map(T F(T)) const; // TODO const

    Deque &where(bool F(T)) const;

    T reduce(T F(T, T), T start) const;

    Deque &concat(Deque &) const;

    Deque &subDeque(int start, int end) const;

    Sequence<T> *toSequence() const;

    Deque &push_back(T item);

    Deque &push_front(T item);

    T front() const;

    T back() const;

    Deque &pop_back();

    Deque &pop_front();

    int size() const;

    explicit operator bool() const;

private:
    Sequence<T> *data;
};


template<class T>
bool operator==(const Deque<T> &d1, const Deque<T> &d2);

template<class T>
bool operator!=(const Deque<T> &d1, const Deque<T> &d2);

template<class T>
std::ostream &operator<<(std::ostream &os, const Deque<T> &d);

#include "Deque.tpp"

#endif
