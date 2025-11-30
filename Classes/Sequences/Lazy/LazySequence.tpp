#pragma once

#include <stdexcept>
#include "../Array/ArraySequences.h"

template<class T>
void LazySequence<T>::initCache() {
    if (source == nullptr) {
        cachedSize = 0;
        cache = nullptr;
        computed = nullptr;
        return;
    }

    int size = isInfinite ? 1000 : source->getSize(); // Для бесконечных - ограничиваем кэш
    cache = new T[size];
    computed = new bool[size];
    for (int i = 0; i < size; i++) {
        computed[i] = false;
    }
    cachedSize = size;
}

template<class T>
LazySequence<T>::LazySequence(Sequence<T> *source) {
    this->source = source;
    this->mapFunc = nullptr;
    this->whereFunc = nullptr;
    if (source != nullptr && source->getSize() == -1) {
        this->isInfinite = true;
    } else {
        this->isInfinite = false;
    }
    initCache();
}

template<class T>
LazySequence<T>::LazySequence(T *items, int count) {
    this->source = new MutableArraySequence<T>(items, count);
    this->mapFunc = nullptr;
    this->whereFunc = nullptr;
    this->isInfinite = false;
    initCache();
}

template<class T>
LazySequence<T>::LazySequence() {
    this->source = new MutableArraySequence<T>();
    this->mapFunc = nullptr;
    this->whereFunc = nullptr;
    this->isInfinite = false;
    initCache();
}

template<class T>
LazySequence<T>::~LazySequence() {
    if (cache != nullptr) {
        delete[] cache;
    }
    if (computed != nullptr) {
        delete[] computed;
    }
    if (source != nullptr) {
        delete source;
    }
}

template<class T>
T LazySequence<T>::computeValue(int index) const {
    if (source == nullptr) {
        throw std::out_of_range("index out of range");
    }

    // Получаем значение из источника
    T value;

    // Если есть where фильтр, нужно найти элемент по отфильтрованному индексу
    if (whereFunc != nullptr) {
        int filteredIndex = 0;
        int sourceSize = source->getSize();
        int maxIterations = (sourceSize == -1) ? 10000 : sourceSize; // Для бесконечных ограничиваем
        
        for (int i = 0; i < maxIterations; i++) {
            T temp = source->get(i);
            if (whereFunc(temp)) {
                if (filteredIndex == index) {
                    value = temp;
                    break;
                }
                filteredIndex++;
            }
        }
        if (filteredIndex != index) {
            throw std::out_of_range("index out of range after filtering");
        }
    } else {
        // Без фильтра просто берем из источника
        value = source->get(index);
    }

    // Применяем map функцию
    if (mapFunc != nullptr) {
        value = mapFunc(value);
    }

    return value;
}

template<class T>
T &LazySequence<T>::get(int index) const {
    if (source == nullptr) {
        throw std::out_of_range("index out of range");
    }

    // Для бесконечных последовательностей расширяем кэш при необходимости
    if (isInfinite && index >= cachedSize) {
        // Расширяем кэш
        int newSize = index + 100;
        T *newCache = new T[newSize];
        bool *newComputed = new bool[newSize];
        
        // Копируем старый кэш
        for (int i = 0; i < cachedSize; i++) {
            newCache[i] = cache[i];
            newComputed[i] = computed[i];
        }
        
        // Инициализируем новые элементы
        for (int i = cachedSize; i < newSize; i++) {
            newComputed[i] = false;
        }
        
        delete[] cache;
        delete[] computed;
        cache = newCache;
        computed = newComputed;
        cachedSize = newSize;
    }

    if (index < 0 || (!isInfinite && index >= cachedSize)) {
        throw std::out_of_range("index out of range");
    }

    // Если еще не вычислено, вычисляем и сохраняем в кэш
    if (!computed[index]) {
        cache[index] = computeValue(index);
        computed[index] = true;
    }

    // Возвращаем ссылку на элемент кэша
    return cache[index];
}

template<class T>
T &LazySequence<T>::getFirst() {
    return get(0);
}

template<class T>
T &LazySequence<T>::getLast() {
    if (isInfinite) {
        throw std::runtime_error("Cannot get last element of infinite sequence");
    }
    return get(getSize() - 1);
}

template<class T>
int LazySequence<T>::getSize() const {
    if (isInfinite) {
        return -1; // Специальное значение для бесконечных
    }
    if (source == nullptr) {
        return 0;
    }

    int sourceSize = source->getSize();
    if (sourceSize == -1) {
        // Источник бесконечен
        return -1;
    }

    // Если есть where фильтр, нужно посчитать отфильтрованные элементы
    if (whereFunc != nullptr) {
        int count = 0;
        for (int i = 0; i < sourceSize; i++) {
            if (whereFunc(source->get(i))) {
                count++;
            }
        }
        return count;
    }

    return sourceSize;
}

template<class T>
T &LazySequence<T>::operator[](int index) {
    if (-index > getSize() && !isInfinite) throw std::out_of_range("index out of range");
    if (index < 0 && !isInfinite) index += getSize();
    return get(index);
}

template<class T>
Sequence<T> *LazySequence<T>::append(T item) {
    return instance()->inAppend(item);
}

template<class T>
Sequence<T> *LazySequence<T>::prepend(T item) {
    return instance()->inPrepend(item);
}

template<class T>
Sequence<T> *LazySequence<T>::insertAt(T item, int index) {
    return instance()->inInsertAt(item, index);
}

template<class T>
Sequence<T> *LazySequence<T>::getSubsequence(int startIndex, int endIndex) {
    return instance()->inGetSubsequence(startIndex, endIndex);
}

template<class T>
Sequence<T> *LazySequence<T>::inGetSubsequence(int startIndex, int endIndex) {
    // Создаем новую последовательность с подпоследовательностью
    auto *result = new MutableArraySequence<T>();
    for (int i = startIndex; i <= endIndex; i++) {
        result->append(get(i));
    }
    delete source;
    source = result;
    initCache();
    return static_cast<Sequence<T>*>(this);
}

template<class T>
Sequence<T> *LazySequence<T>::set(int index, T value) {
    return instance()->inSet(index, value);
}

template<class T>
Sequence<T> *LazySequence<T>::inSet(int index, T item) {
    if (source != nullptr) {
        source->inSet(index, item);
        // Инвалидируем кэш для этого индекса
        if (index < cachedSize) {
            computed[index] = false;
        }
    }
    return this;
}

template<class T>
Sequence<T> *LazySequence<T>::remove(int index) {
    return instance()->inRemove(index);
}

template<class T>
Sequence<T> *LazySequence<T>::inRemove(int index) {
    if (source != nullptr) {
        source->inRemove(index);
        initCache(); // Пересоздаем кэш
    }
    return this;
}

template<class T>
bool LazySequence<T>::contains(T item) {
    for (int i = 0; i < getSize(); i++) {
        if (get(i) == item) {
            return true;
        }
    }
    return false;
}

template<class T>
int LazySequence<T>::find(T item) {
    for (int i = 0; i < getSize(); i++) {
        if (get(i) == item) {
            return i;
        }
    }
    return getSize(); // Не найдено
}

template<class T>
Sequence<T> *LazySequence<T>::construct() {
    return new LazySequence<T>();
}

template<class T>
Sequence<T> *LazySequence<T>::inAppend(T item) {
    if (source != nullptr) {
        source->inAppend(item);
        initCache(); // Пересоздаем кэш
    }
    return static_cast<Sequence<T>*>(this);
}

template<class T>
Sequence<T> *LazySequence<T>::inPrepend(T item) {
    if (source != nullptr) {
        source->inPrepend(item);
        initCache(); // Пересоздаем кэш
    }
    return static_cast<Sequence<T>*>(this);
}

template<class T>
Sequence<T> *LazySequence<T>::inInsertAt(T item, int index) {
    if (source != nullptr) {
        source->inInsertAt(item, index);
        initCache(); // Пересоздаем кэш
    }
    return static_cast<Sequence<T>*>(this);
}

template<class T>
LazySequence<T> *LazySequence<T>::lazyMap(T func(T)) {
    this->mapFunc = func;
    // Инвалидируем весь кэш
    for (int i = 0; i < cachedSize; i++) {
        computed[i] = false;
    }
    return this;
}

template<class T>
LazySequence<T> *LazySequence<T>::lazyWhere(bool func(T)) {
    this->whereFunc = func;
    // Инвалидируем весь кэш
    for (int i = 0; i < cachedSize; i++) {
        computed[i] = false;
    }
    return this;
}

template<class T>
void LazySequence<T>::setInfinite(bool infinite) {
    this->isInfinite = infinite;
    if (infinite) {
        initCache(); // Пересоздаем кэш с большим размером
    }
}

template<class T>
Sequence<T> *LazySequence<T>::clone() {
    auto *result = new LazySequence<T>(source);
    result->mapFunc = this->mapFunc;
    result->whereFunc = this->whereFunc;
    result->isInfinite = this->isInfinite;
    return result;
}

template<class T>
Sequence<T> *LazySequence<T>::instance() {
    return this;
}

