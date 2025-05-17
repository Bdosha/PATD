#ifndef STACK_H
#define STACK_H
#include "Deque.h"
#pragma once


template<class T>
class Stack {
public:
    Stack() {
        data = new Deque<T>();
    }

    explicit Stack(Sequence<T> *seq) {
        data = new Deque<T>(seq);
    }

    Sequence<T> *toSequence() const {
        return data->toSequence();
    }

    Stack(const Stack &q) {
        data = new Deque<T>(q.toSequence());
    }

    Stack(T *items, int size) {
        data = new Deque<T>(items, size);
    }

    explicit Stack(const Deque<T> &q) {
        Queue(q.toSequence());
    }

    ~Stack() {
        delete data;
    }


    Stack &map(T F(T)) {
        return *new Stack(data->map(F).toSequence());
    }

    Stack &where(bool F(T)) {
        return *new Stack(data->where(F).toSequence());
    }

    T reduce(T F(T, T), T start) {
        return data->reduce(F, start);
    }

    Stack &concat(Stack q) {
        return *new Stack(data->concat(q));
        // return new Queue(data->concat(q->toSequence()));
    }

    Stack &subQueue(int start, int end) {
        return *new Stack(data->subDeque(start, end).toSequence());
    }


    Stack &push(T item) {
        data->push_back(item);
        return *this;
    }

    Stack &pop() {
        data->pop_back();
        return *this;
    }

    [[nodiscard]] int size() const {
        return data->size();
    }

    T back() const {
        return data->back();
    }

    explicit operator bool() const {
        return size() > 0;
    }

private:
    Deque<T> *data;
};


template<class T>
bool operator==(const Stack<T> &s1, const Stack<T> &s2) {
    return *s1.toSequence() == *s2.toSequence();
}

template<class T>
bool operator!=(const Stack<T> &s1, const Stack<T> &s2) {
    return !(s1 == s2);
}

template<class T>
std::ostream &operator<<(std::ostream &os, const Stack<T> &s) {
    os << s.toSequence();
    return os;
}


#endif
