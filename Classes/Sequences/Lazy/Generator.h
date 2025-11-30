#ifndef GENERATOR_H
#define GENERATOR_H

#include "../Sequence.h"

template<class T>
class Generator : public Sequence<T> {
private:
    T (*generateFunc)(int index);
    T startValue;
    T stepValue;
    bool isRangeGenerator;

    Sequence<T> *clone() override;

    Sequence<T> *instance() override;

public:
    // Конструктор с функцией генерации
    explicit Generator(T (*func)(int));

    // Конструктор для range
    Generator(T start, T step);

    // Конструктор для repeat
    explicit Generator(T value);

    ~Generator() override = default;

    T &get(int index) const override;

    [[nodiscard]] int getSize() const override;

    Sequence<T> *construct() override;

    Sequence<T> *inSet(int index, T item) override;

    Sequence<T> *inAppend(T item) override;

    Sequence<T> *inPrepend(T item) override;

    Sequence<T> *inInsertAt(T item, int index) override;

    Sequence<T> *inGetSubsequence(int startIndex, int endIndex) override;

    Sequence<T> *inRemove(int index) override;

    Sequence<T> *set(int index, T value) override;

    Sequence<T> *append(T item) override;

    Sequence<T> *prepend(T item) override;

    Sequence<T> *insertAt(T item, int index) override;

    Sequence<T> *getSubsequence(int startIndex, int endIndex) override;

    Sequence<T> *remove(int index) override;

    bool contains(T item) override;

    int find(T item) override;

    T &getFirst() override;

    T &getLast() override;

    T &operator[](int index) override;

    // Статические методы для создания генераторов
    static Generator<T> *range(T start, T step);

    static Generator<T> *repeat(T value);
};

#include "Generator.tpp"

#endif

