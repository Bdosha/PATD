#pragma once

#include <stdexcept>

template<class T>
Generator<T>::Generator(T (*func)(int)) {
    this->generateFunc = func;
    this->isRangeGenerator = false;
    this->startValue = T();
    this->stepValue = T();
}

template<class T>
Generator<T>::Generator(T start, T step) {
    this->startValue = start;
    this->stepValue = step;
    this->isRangeGenerator = true;
    this->generateFunc = nullptr;
}

template<class T>
Generator<T>::Generator(T value) {
    this->startValue = value;
    this->stepValue = T();
    this->isRangeGenerator = false;
    this->generateFunc = nullptr;
}

template<class T>
T &Generator<T>::get(int index) const {
    if (index < 0) {
        throw std::out_of_range("index out of range");
    }

    static T temp;

    if (isRangeGenerator) {
        // Для range: start + index * step
        temp = startValue + static_cast<T>(index) * stepValue;
    } else if (generateFunc != nullptr) {
        // Для пользовательской функции
        temp = generateFunc(index);
    } else {
        // Для repeat
        temp = startValue;
    }

    return temp;
}

template<class T>
int Generator<T>::getSize() const {
    return -1; // Бесконечная последовательность
}

template<class T>
Sequence<T> *Generator<T>::construct() {
    if (isRangeGenerator) {
        return new Generator<T>(startValue, stepValue);
    } else if (generateFunc != nullptr) {
        return new Generator<T>(generateFunc);
    } else {
        return new Generator<T>(startValue);
    }
}

template<class T>
T &Generator<T>::getFirst() {
    return get(0);
}

template<class T>
T &Generator<T>::getLast() {
    throw std::runtime_error("Cannot get last element of infinite sequence");
}

template<class T>
T &Generator<T>::operator[](int index) {
    if (index < 0) throw std::out_of_range("index out of range");
    return get(index);
}

template<class T>
Sequence<T> *Generator<T>::set(int index, T value) {
    throw std::runtime_error("Cannot modify infinite generator");
}

template<class T>
Sequence<T> *Generator<T>::inSet(int index, T item) {
    throw std::runtime_error("Cannot modify infinite generator");
}

template<class T>
Sequence<T> *Generator<T>::append(T item) {
    throw std::runtime_error("Cannot modify infinite generator");
}

template<class T>
Sequence<T> *Generator<T>::prepend(T item) {
    throw std::runtime_error("Cannot modify infinite generator");
}

template<class T>
Sequence<T> *Generator<T>::insertAt(T item, int index) {
    throw std::runtime_error("Cannot modify infinite generator");
}

template<class T>
Sequence<T> *Generator<T>::getSubsequence(int startIndex, int endIndex) {
    throw std::runtime_error("Cannot get subsequence from infinite generator");
}

template<class T>
Sequence<T> *Generator<T>::inGetSubsequence(int startIndex, int endIndex) {
    throw std::runtime_error("Cannot get subsequence from infinite generator");
}

template<class T>
Sequence<T> *Generator<T>::remove(int index) {
    throw std::runtime_error("Cannot modify infinite generator");
}

template<class T>
Sequence<T> *Generator<T>::inRemove(int index) {
    throw std::runtime_error("Cannot modify infinite generator");
}

template<class T>
Sequence<T> *Generator<T>::inAppend(T item) {
    throw std::runtime_error("Cannot modify infinite generator");
}

template<class T>
Sequence<T> *Generator<T>::inPrepend(T item) {
    throw std::runtime_error("Cannot modify infinite generator");
}

template<class T>
Sequence<T> *Generator<T>::inInsertAt(T item, int index) {
    throw std::runtime_error("Cannot modify infinite generator");
}

template<class T>
bool Generator<T>::contains(T item) {
    // Для бесконечной последовательности проверяем первые N элементов
    for (int i = 0; i < 1000; i++) {
        if (get(i) == item) {
            return true;
        }
    }
    return false;
}

template<class T>
int Generator<T>::find(T item) {
    // Для бесконечной последовательности ищем в первых N элементах
    for (int i = 0; i < 1000; i++) {
        if (get(i) == item) {
            return i;
        }
    }
    return -1; // Не найдено
}

template<class T>
Generator<T> *Generator<T>::range(T start, T step) {
    return new Generator<T>(start, step);
}

template<class T>
Generator<T> *Generator<T>::repeat(T value) {
    return new Generator<T>(value);
}

template<class T>
Sequence<T> *Generator<T>::clone() {
    if (isRangeGenerator) {
        return new Generator<T>(startValue, stepValue);
    } else if (generateFunc != nullptr) {
        return new Generator<T>(generateFunc);
    } else {
        return new Generator<T>(startValue);
    }
}

template<class T>
Sequence<T> *Generator<T>::instance() {
    return this;
}

