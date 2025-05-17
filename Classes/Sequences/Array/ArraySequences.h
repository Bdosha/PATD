#ifndef ARRAYSEQUENCES_H
#define ARRAYSEQUENCES_H
#include "basic/ArraySequence.h"

template<class T>
class ImmutableArraySequence final : public ArraySequence<T> {
public:
    ImmutableArraySequence() : ArraySequence<T>() {
    }

    ImmutableArraySequence(T *items, int count) : ArraySequence<T>(items, count) {
    }

    explicit ImmutableArraySequence(DynamicArray<T> other) : ArraySequence<T>(other) {
    }

    explicit ImmutableArraySequence(int size) : ArraySequence<T>(size) {
    }

    Sequence<T> *construct() override {
        return new ImmutableArraySequence();
    }

private:
    Sequence<T> *clone() override {
        int a = this->getSize();
        if (this->getSize() == 0) return new ImmutableArraySequence();

        T *items = new T[a];
        for (int i = 0; i < this->getSize(); i++) {
            items[i] = this->get(i);
        }
        return new ImmutableArraySequence(items, a);
    }

    Sequence<T> *instance() override {
        return this->clone();
    }
};


template<class T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence() : ArraySequence<T>() {
    }

    MutableArraySequence(T *items, int count) : ArraySequence<T>(items, count) {
    }

    explicit MutableArraySequence(DynamicArray<T> arr) : ArraySequence<T>(arr) {
    }

    explicit MutableArraySequence(Sequence<T> *other) : ArraySequence<T>(other) {
    }


    explicit MutableArraySequence(int size) : ArraySequence<T>(size) {
    }

    Sequence<T> *construct() override {
        return new ImmutableArraySequence<T>();
    }

private:
    Sequence<T> *instance() override {
        return this;
    }

    Sequence<T> *clone() override {
        return nullptr;
    }
};

#endif
