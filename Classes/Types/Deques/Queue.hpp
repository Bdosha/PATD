#ifndef QUEUE_H
#define QUEUE_H
#include "Deque.h"
#pragma once


template<class T>
class Queue {
public:
    Queue() {
        data = new Deque<T>();
    }

    explicit Queue(Sequence<T> *seq) {
        data = new Deque<T>(seq);
    }

    Sequence<T> *toSequence() const {
        return data->toSequence();
    }

    Queue(const Queue &q) {
        data = new Deque<T>(q.toSequence());
    }

    Queue(T *items, int size) {
        data = new Deque<T>(items, size);
    }

    explicit Queue(const Deque<T> &q) {
        Queue(q.toSequence());
    }

    ~Queue() {
        delete data;
    }


    Queue &map(T F(T)) const {
        return *new Queue(data->map(F).toSequence());
    }

    Queue &where(bool F(T)) const {
        return *new Queue(data->where(F).toSequence());
    }

    T reduce(T F(T, T), T start) const {
        return data->reduce(F, start);
    }

    Queue &concat(Queue q) const {
        return *new Queue(data->concat(q));
    }

    Queue &subQueue(int start, int end) const {
        return *new Queue(data->subDeque(start, end).toSequence());
    }


    Queue &push(T item) {
        data->push_back(item);
        return *this;
    }

    Queue &pop() {
        data->pop_front();
        return *this;
    }

    int size() const {
        return data->size();
    }

    T front() const {
        return data->front();
    }

    explicit operator bool() const {
        return size() > 0;
    }

private:
    Deque<T> *data;
};


template<class T>
bool operator==(const Queue<T> &q1, const Queue<T> &q2) {
    return *q1.toSequence() == *q2.toSequence();
}

template<class T>
bool operator!=(const Queue<T> &q1, const Queue<T> &q2) {
    return !(q1 == q2);
}

template<class T>
std::ostream &operator<<(std::ostream &os, const Queue<T> &q) {
    os << q.toSequence();
    return os;
}


#endif
