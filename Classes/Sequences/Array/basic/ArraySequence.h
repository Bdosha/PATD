#ifndef ARRAYSEQUENCE_H
#define ARRAYSEQUENCE_H
#include "../capsule/DynamicArray.h"


template<class T>
class ArraySequence : public Sequence<T> {
public:
    explicit ArraySequence(DynamicArray<T> &other);

    explicit ArraySequence(Sequence<T> *other);

    ArraySequence();

    ArraySequence(T *items, int count);

    explicit ArraySequence(int count);

    T &getFirst() override;

    T &getLast() override;

    T &get(int index)const override;

    T &operator[](int index) override;

    [[nodiscard]] int getSize()const override;

    Sequence<T> *append(T item) override;

    Sequence<T> *prepend(T item) override;

    Sequence<T> *insertAt(T item, int index) override;


    Sequence<T> *getSubsequence(int startIndex, int endIndex) override;

    Sequence<T> *set(int index, T value) override;

    Sequence<T> *remove(int) override;

    bool contains(T) override;

    int find(T) override;

private:
    DynamicArray<T> *list;

    Sequence<T> *inAppend(T item) override;

    Sequence<T> *inSet(int index, T item) override;

    Sequence<T> *inPrepend(T item) override;

    Sequence<T> *inInsertAt(T item, int index) override;

    Sequence<T> *inGetSubsequence(int startIndex, int endIndex) override;

    Sequence<T> *inRemove(int) override;

    Sequence<T> *clone() override =0;

    Sequence<T> *instance() override =0;
};

#include "ArraySequence.tpp"
#endif
