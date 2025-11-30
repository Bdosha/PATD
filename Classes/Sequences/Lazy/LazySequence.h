#ifndef LAZYSEQUENCE_H
#define LAZYSEQUENCE_H

#include "../Sequence.h"

template<class T>
class LazySequence : public Sequence<T> {
private:
    Sequence<T> *source;
    T (*mapFunc)(T);
    bool (*whereFunc)(T);
    mutable T *cache;
    mutable bool *computed;
    mutable int cachedSize;
    bool isInfinite;

    Sequence<T> *clone() override;

    Sequence<T> *instance() override;

    T computeValue(int index) const;

public:
    explicit LazySequence(Sequence<T> *source);

    LazySequence(T *items, int count);

    LazySequence();

    ~LazySequence() override;

    T &getFirst() override;

    T &getLast() override;

    T &get(int index) const override;

    T &operator[](int index) override;

    [[nodiscard]] int getSize() const override;

    Sequence<T> *append(T item) override;

    Sequence<T> *prepend(T item) override;

    Sequence<T> *insertAt(T item, int index) override;

    Sequence<T> *getSubsequence(int startIndex, int endIndex) override;

    Sequence<T> *inGetSubsequence(int startIndex, int endIndex) override;

    Sequence<T> *set(int index, T value) override;

    Sequence<T> *inSet(int index, T item) override;

    Sequence<T> *remove(int index) override;

    Sequence<T> *inRemove(int index) override;

    bool contains(T item) override;

    int find(T item) override;

    Sequence<T> *construct() override;

    Sequence<T> *inAppend(T item) override;

    Sequence<T> *inPrepend(T item) override;

    Sequence<T> *inInsertAt(T item, int index) override;

    // Ленивые операции
    LazySequence<T> *lazyMap(T func(T));

    LazySequence<T> *lazyWhere(bool func(T));

    void setInfinite(bool infinite);

    void initCache();
};

#include "LazySequence.tpp"

#endif

