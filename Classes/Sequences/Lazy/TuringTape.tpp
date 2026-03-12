#pragma once
#include "TuringTape.h"
#include "BidirectionalGenerator.h"
#include "../Array/ArraySequences.h"
#include <stdexcept>
#include <algorithm>

template<class T>
TuringTape<T>::TuringTape(T initialValue)
    : leftGenerator(nullptr),
      rightGenerator(nullptr),
      leftMaterialized(new MutableArraySequence<T>()),
      rightMaterialized(new MutableArraySequence<T>()),
      isLeftInfinite(false),
      isRightInfinite(false),
      leftLength(Cardinal(0)),
      rightLength(Cardinal(1)) {
    // Инициализируем правую сторону с начальным значением в позиции 0
    rightMaterialized->inAppend(initialValue);
    
    // Создаем простые генераторы (пустые для левой, с одним элементом для правой)
    Sequence<T> *emptySeq = new MutableArraySequence<T>();
    Sequence<T> *rightSeq = new MutableArraySequence<T>();
    rightSeq->inAppend(initialValue);
    
    leftGenerator = new BidirectionalGenerator<T>(this, emptySeq, true, 0);
    leftGenerator->setInfinite(false);
    leftGenerator->setHasEnd(true);
    leftGenerator->setEndIndex(0);
    
    rightGenerator = new BidirectionalGenerator<T>(this, rightSeq, false, 0);
    rightGenerator->setInfinite(false);
    rightGenerator->setHasEnd(true);
    rightGenerator->setEndIndex(1);
    
    delete emptySeq;
    delete rightSeq;
}

template<class T>
TuringTape<T>::TuringTape(T *items, int count, int startIndex)
    : leftMaterialized(new MutableArraySequence<T>()),
      rightMaterialized(new MutableArraySequence<T>()),
      isLeftInfinite(false),
      isRightInfinite(false) {
    
    if (items == nullptr || count < 0) {
        leftLength = Cardinal(0);
        rightLength = Cardinal(0);
        Sequence<T> *emptySeq = new MutableArraySequence<T>();
        leftGenerator = new BidirectionalGenerator<T>(this, emptySeq, true, 0);
        rightGenerator = new BidirectionalGenerator<T>(this, emptySeq, false, 0);
        delete emptySeq;
        return;
    }
    
    // Разделяем элементы на левую и правую стороны
    int leftCount = 0;
    int rightCount = 0;
    
    // Элементы с индексами < startIndex идут в левую сторону
    // Элементы с индексами >= startIndex идут в правую сторону
    for (int i = 0; i < count; i++) {
        int actualIndex = startIndex + i;
        if (actualIndex < 0) {
            leftCount++;
        } else {
            rightCount++;
        }
    }
    
    // Создаем последовательности для генераторов
    Sequence<T> *leftSeq = new MutableArraySequence<T>();
    Sequence<T> *rightSeq = new MutableArraySequence<T>();
    
    // Собираем элементы левой стороны
    // Элементы должны быть в leftMaterialized в порядке от меньших к большим индексам
    // leftMaterialized[0] = элемент с индексом startIndex (если startIndex < 0)
    // leftMaterialized[i] = элемент с индексом startIndex + i
    
    for (int i = 0; i < count; i++) {
        int actualIndex = startIndex + i;
        if (actualIndex < 0) {
            // Для левой стороны: добавляем в порядке появления (от меньших к большим индексам)
            // Используем inAppend, чтобы сохранить порядок
            leftMaterialized->inAppend(items[i]);
        } else {
            rightSeq->inAppend(items[i]);
            rightMaterialized->inAppend(items[i]);
        }
    }
    
    // Создаем leftSeq в том же порядке, что и leftMaterialized
    for (int i = 0; i < static_cast<int>(leftMaterialized->getSize()); i++) {
        leftSeq->inAppend(leftMaterialized->get(i));
    }
    
    // Устанавливаем длины
    leftLength = Cardinal(leftCount);
    rightLength = Cardinal(rightCount);
    
    // Создаем генераторы
    createSimpleGenerators(leftSeq, rightSeq, startIndex);
    
    delete leftSeq;
    delete rightSeq;
}

template<class T>
TuringTape<T>::TuringTape(Sequence<T> *seq, int startIndex)
    : leftMaterialized(new MutableArraySequence<T>()),
      rightMaterialized(new MutableArraySequence<T>()),
      isLeftInfinite(false),
      isRightInfinite(false) {
    
    if (seq == nullptr) {
        leftLength = Cardinal(0);
        rightLength = Cardinal(0);
        Sequence<T> *emptySeq = new MutableArraySequence<T>();
        leftGenerator = new BidirectionalGenerator<T>(this, emptySeq, true, 0);
        rightGenerator = new BidirectionalGenerator<T>(this, emptySeq, false, 0);
        delete emptySeq;
        return;
    }
    
    int seqSize = seq->getSize();
    
    // Разделяем на левую и правую стороны
    Sequence<T> *leftSeq = new MutableArraySequence<T>();
    Sequence<T> *rightSeq = new MutableArraySequence<T>();
    
    int leftCount = 0;
    int rightCount = 0;
    
    // Собираем элементы левой стороны
    for (int i = 0; i < seqSize; i++) {
        int actualIndex = startIndex + i;
        if (actualIndex < 0) {
            leftMaterialized->inAppend(seq->get(i));
            leftCount++;
        } else {
            rightSeq->inAppend(seq->get(i));
            rightMaterialized->inAppend(seq->get(i));
            rightCount++;
        }
    }
    
    // Создаем leftSeq в том же порядке
    for (int i = 0; i < static_cast<int>(leftMaterialized->getSize()); i++) {
        leftSeq->inAppend(leftMaterialized->get(i));
    }
    
    leftLength = Cardinal(leftCount);
    rightLength = Cardinal(rightCount);
    
    createSimpleGenerators(leftSeq, rightSeq, startIndex);
    
    delete leftSeq;
    delete rightSeq;
}

template<class T>
TuringTape<T>::TuringTape(std::function<T(Sequence<T>*)> leftRule,
                          std::function<T(Sequence<T>*)> rightRule,
                          Sequence<T> *initialLeftElements,
                          Sequence<T> *initialRightElements)
    : leftMaterialized(new MutableArraySequence<T>()),
      rightMaterialized(new MutableArraySequence<T>()),
      isLeftInfinite(true),
      isRightInfinite(true),
      leftLength(Cardinal::Infinity()),
      rightLength(Cardinal::Infinity()) {
    
    createRecurrentGenerators(leftRule, rightRule, initialLeftElements, initialRightElements);
}

template<class T>
TuringTape<T>::TuringTape(const TuringTape<T> &other)
    : leftMaterialized(new MutableArraySequence<T>(other.leftMaterialized)),
      rightMaterialized(new MutableArraySequence<T>(other.rightMaterialized)),
      isLeftInfinite(other.isLeftInfinite),
      isRightInfinite(other.isRightInfinite),
      leftLength(other.leftLength),
      rightLength(other.rightLength) {
    
    // Копируем генераторы
    if (other.leftGenerator != nullptr) {
        leftGenerator = new BidirectionalGenerator<T>(*other.leftGenerator);
        leftGenerator->setOwner(this);
    } else {
        leftGenerator = nullptr;
    }
    
    if (other.rightGenerator != nullptr) {
        rightGenerator = new BidirectionalGenerator<T>(*other.rightGenerator);
        rightGenerator->setOwner(this);
    } else {
        rightGenerator = nullptr;
    }
}

template<class T>
TuringTape<T>::~TuringTape() {
    if (leftGenerator != nullptr) {
        leftGenerator->setOwner(nullptr);
        delete leftGenerator;
    }
    if (rightGenerator != nullptr) {
        rightGenerator->setOwner(nullptr);
        delete rightGenerator;
    }
    if (leftMaterialized != nullptr) {
        delete leftMaterialized;
    }
    if (rightMaterialized != nullptr) {
        delete rightMaterialized;
    }
}

template<class T>
int TuringTape<T>::leftIndexToMaterialized(int index) const {
    // index отрицательный, например -2
    // leftMaterialized хранит элементы в порядке от меньших к большим индексам
    // Если leftMaterialized создан из массива с startIndex = -3, то:
    // leftMaterialized[0] = элемент с индексом -3
    // leftMaterialized[1] = элемент с индексом -2
    // leftMaterialized[2] = элемент с индексом -1
    
    // Но мы не знаем startIndex, поэтому используем предположение:
    // leftMaterialized[0] = элемент с индексом -leftSize
    // leftMaterialized[i] = элемент с индексом -leftSize + i
    
    int leftSize = static_cast<int>(leftMaterialized->getSize());
    if (leftSize == 0) {
        return -1;
    }
    
    // Минимальный индекс в leftMaterialized (предполагаем, что это -leftSize)
    int minIndex = -leftSize;
    
    // Преобразуем абсолютный индекс в индекс для leftMaterialized
    int materializedIndex = index - minIndex;
    
    if (materializedIndex < 0 || materializedIndex >= leftSize) {
        return -1;
    }
    
    return materializedIndex;
}

template<class T>
int TuringTape<T>::rightIndexToMaterialized(int index) const {
    // index неотрицательный
    // rightMaterialized[0] = элемент с индексом 0
    // rightMaterialized[i] = элемент с индексом i
    
    if (index < 0) {
        return -1;
    }
    
    int rightSize = static_cast<int>(rightMaterialized->getSize());
    if (index >= rightSize) {
        return -1;
    }
    
    return index;
}

template<class T>
void TuringTape<T>::materializeLeftUpTo(int index) {
    if (index > 0) {
        throw std::out_of_range("Left side index must be negative or zero");
    }
    
    if (leftMaterialized == nullptr) {
        throw std::runtime_error("Left materialized sequence is null");
    }
    
    int currentMinIndex = -static_cast<int>(leftMaterialized->getSize());
    
    // Материализуем элементы от currentMinIndex до index включительно
    while (index < currentMinIndex) {
        if (leftGenerator == nullptr || !leftGenerator->HasNext()) {
            if (!isLeftInfinite) {
                throw std::out_of_range("Cannot materialize beyond left end");
            }
            if (leftGenerator == nullptr) {
                throw std::runtime_error("Left generator is null");
            }
        }
        
        try {
            T next = leftGenerator->GetNext();
            leftMaterialized->inPrepend(next);
            currentMinIndex--;
        } catch (const std::out_of_range &) {
            if (!isLeftInfinite) {
                isLeftInfinite = false;
                leftLength = Cardinal(leftMaterialized->getSize());
            }
            break;
        }
    }
}

template<class T>
void TuringTape<T>::materializeRightUpTo(int index) {
    if (index < 0) {
        throw std::out_of_range("Right side index must be non-negative");
    }
    
    if (rightMaterialized == nullptr) {
        throw std::runtime_error("Right materialized sequence is null");
    }
    
    int currentMaxIndex = static_cast<int>(rightMaterialized->getSize()) - 1;
    
    // Материализуем элементы от currentMaxIndex до index включительно
    while (index > currentMaxIndex) {
        if (rightGenerator == nullptr) {
            throw std::runtime_error("Right generator is null");
        }
        
        if (!rightGenerator->HasNext()) {
            if (!isRightInfinite) {
                throw std::out_of_range("Cannot materialize beyond right end");
            }
            // Для бесконечной ленты продолжаем генерировать
            // Генератор должен быть способен генерировать бесконечно
        }
        
        try {
            T next = rightGenerator->GetNext();
            rightMaterialized->inAppend(next);
            currentMaxIndex++;
        } catch (const std::out_of_range &) {
            if (!isRightInfinite) {
                isRightInfinite = false;
                rightLength = Cardinal(rightMaterialized->getSize());
            }
            break;
        }
    }
}

template<class T>
T TuringTape<T>::Get(int index) {
    if (index < 0) {
        // Левая сторона
        materializeLeftUpTo(index);
        int matIndex = leftIndexToMaterialized(index);
        if (matIndex < 0) {
            throw std::out_of_range("Index out of range");
        }
        return leftMaterialized->get(matIndex);
    } else {
        // Правая сторона
        materializeRightUpTo(index);
        int matIndex = rightIndexToMaterialized(index);
        if (matIndex < 0) {
            throw std::out_of_range("Index out of range");
        }
        return rightMaterialized->get(matIndex);
    }
}

template<class T>
void TuringTape<T>::Set(int index, T value) {
    if (index < 0) {
        // Левая сторона
        materializeLeftUpTo(index);
        int matIndex = leftIndexToMaterialized(index);
        if (matIndex < 0) {
            throw std::out_of_range("Index out of range");
        }
        leftMaterialized->inSet(matIndex, value);
    } else {
        // Правая сторона
        materializeRightUpTo(index);
        int matIndex = rightIndexToMaterialized(index);
        if (matIndex < 0) {
            throw std::out_of_range("Index out of range");
        }
        rightMaterialized->inSet(matIndex, value);
    }
}

template<class T>
TuringTape<T> *TuringTape<T>::InsertAt(int index, T item) {
    auto *result = new TuringTape<T>(*this);
    
    if (index < 0) {
        result->materializeLeftUpTo(index);
        int matIndex = result->leftIndexToMaterialized(index);
        if (matIndex < 0) {
            delete result;
            throw std::out_of_range("Index out of range");
        }
        result->leftMaterialized->inInsertAt(item, matIndex);
        if (!result->isLeftInfinite) {
            result->leftLength = Cardinal(result->leftMaterialized->getSize());
        }
    } else {
        result->materializeRightUpTo(index);
        int matIndex = result->rightIndexToMaterialized(index);
        if (matIndex < 0) {
            delete result;
            throw std::out_of_range("Index out of range");
        }
        result->rightMaterialized->inInsertAt(item, matIndex);
        if (!result->isRightInfinite) {
            result->rightLength = Cardinal(result->rightMaterialized->getSize());
        }
    }
    
    return result;
}

template<class T>
TuringTape<T> *TuringTape<T>::Remove(int index) {
    auto *result = new TuringTape<T>(*this);
    
    if (index < 0) {
        result->materializeLeftUpTo(index);
        int matIndex = result->leftIndexToMaterialized(index);
        if (matIndex < 0) {
            delete result;
            throw std::out_of_range("Index out of range");
        }
        result->leftMaterialized->inRemove(matIndex);
        if (!result->isLeftInfinite) {
            result->leftLength = Cardinal(result->leftMaterialized->getSize());
        }
    } else {
        result->materializeRightUpTo(index);
        int matIndex = result->rightIndexToMaterialized(index);
        if (matIndex < 0) {
            delete result;
            throw std::out_of_range("Index out of range");
        }
        result->rightMaterialized->inRemove(matIndex);
        if (!result->isRightInfinite) {
            result->rightLength = Cardinal(result->rightMaterialized->getSize());
        }
    }
    
    return result;
}

template<class T>
TuringTape<T> *TuringTape<T>::GetSubsequence(int startIndex, int endIndex) {
    if (startIndex > endIndex) {
        throw std::out_of_range("Start index must be <= end index");
    }
    
    // Материализуем нужные элементы
    if (startIndex < 0) {
        materializeLeftUpTo(startIndex);
    }
    if (endIndex < 0) {
        materializeLeftUpTo(endIndex);
    } else {
        materializeRightUpTo(endIndex);
    }
    
    // Создаем новую ленту
    Sequence<T> *subseq = new MutableArraySequence<T>();
    
    for (int i = startIndex; i <= endIndex; i++) {
        try {
            T elem = Get(i);
            subseq->inAppend(elem);
        } catch (const std::exception &) {
            delete subseq;
            throw std::out_of_range("Cannot get subsequence");
        }
    }
    
    // Определяем startIndex для новой ленты
    int newStartIndex = startIndex;
    auto *result = new TuringTape<T>(subseq, newStartIndex);
    delete subseq;
    
    return result;
}

template<class T>
Cardinal TuringTape<T>::GetLeftLength() const {
    return leftLength;
}

template<class T>
Cardinal TuringTape<T>::GetRightLength() const {
    return rightLength;
}

template<class T>
Cardinal TuringTape<T>::GetTotalLength() const {
    if (leftLength.isInfinity() || rightLength.isInfinity()) {
        return Cardinal::Infinity();
    }
    return leftLength + rightLength;
}

template<class T>
size_t TuringTape<T>::GetLeftMaterializedCount() const {
    if (leftMaterialized == nullptr) {
        return 0;
    }
    return leftMaterialized->getSize();
}

template<class T>
size_t TuringTape<T>::GetRightMaterializedCount() const {
    if (rightMaterialized == nullptr) {
        return 0;
    }
    return rightMaterialized->getSize();
}

template<class T>
size_t TuringTape<T>::GetTotalMaterializedCount() const {
    return GetLeftMaterializedCount() + GetRightMaterializedCount();
}

template<class T>
bool TuringTape<T>::IsLeftInfinite() const {
    return isLeftInfinite;
}

template<class T>
bool TuringTape<T>::IsRightInfinite() const {
    return isRightInfinite;
}

template<class T>
T TuringTape<T>::operator[](int index) {
    return Get(index);
}

template<class T>
template<class T2>
TuringTape<T2> *TuringTape<T>::Map(T2 (*func)(T)) {
    // Собираем преобразованные элементы в последовательности
    Sequence<T2> *mappedLeft = new MutableArraySequence<T2>();
    Sequence<T2> *mappedRight = new MutableArraySequence<T2>();
    
    // Применяем функцию к материализованным элементам
    for (int i = 0; i < static_cast<int>(leftMaterialized->getSize()); i++) {
        T2 mapped = func(leftMaterialized->get(i));
        mappedLeft->inAppend(mapped);
    }
    
    for (int i = 0; i < static_cast<int>(rightMaterialized->getSize()); i++) {
        T2 mapped = func(rightMaterialized->get(i));
        mappedRight->inAppend(mapped);
    }
    
    // Создаем новую ленту из преобразованных элементов
    int startIndex = mappedLeft->getSize() > 0 ? -static_cast<int>(mappedLeft->getSize()) : 0;
    
    // Объединяем последовательности
    Sequence<T2> *combined = new MutableArraySequence<T2>();
    for (int i = 0; i < mappedLeft->getSize(); i++) {
        combined->inAppend(mappedLeft->get(i));
    }
    for (int i = 0; i < mappedRight->getSize(); i++) {
        combined->inAppend(mappedRight->get(i));
    }
    
    auto *result = new TuringTape<T2>(combined, startIndex);
    
    delete mappedLeft;
    delete mappedRight;
    delete combined;
    
    return result;
}

template<class T>
TuringTape<T> *TuringTape<T>::Where(bool (*predicate)(T)) {
    // Собираем отфильтрованные элементы в последовательности
    Sequence<T> *filteredLeft = new MutableArraySequence<T>();
    Sequence<T> *filteredRight = new MutableArraySequence<T>();
    
    // Применяем фильтр к материализованным элементам
    for (int i = 0; i < static_cast<int>(leftMaterialized->getSize()); i++) {
        T elem = leftMaterialized->get(i);
        if (predicate(elem)) {
            filteredLeft->inAppend(elem);
        }
    }
    
    for (int i = 0; i < static_cast<int>(rightMaterialized->getSize()); i++) {
        T elem = rightMaterialized->get(i);
        if (predicate(elem)) {
            filteredRight->inAppend(elem);
        }
    }
    
    // Создаем новую ленту из отфильтрованных элементов
    // Определяем startIndex для левой стороны
    int startIndex = filteredLeft->getSize() > 0 ? -static_cast<int>(filteredLeft->getSize()) : 0;
    
    // Объединяем последовательности
    Sequence<T> *combined = new MutableArraySequence<T>();
    for (int i = 0; i < filteredLeft->getSize(); i++) {
        combined->inAppend(filteredLeft->get(i));
    }
    for (int i = 0; i < filteredRight->getSize(); i++) {
        combined->inAppend(filteredRight->get(i));
    }
    
    auto *result = new TuringTape<T>(combined, startIndex);
    
    delete filteredLeft;
    delete filteredRight;
    delete combined;
    
    return result;
}

template<class T>
void TuringTape<T>::createSimpleGenerators(Sequence<T> *leftSeq, Sequence<T> *rightSeq, int startIndex) {
    // Создаем генераторы для простых последовательностей
    if (leftSeq != nullptr && leftSeq->getSize() > 0) {
        leftGenerator = new BidirectionalGenerator<T>(this, leftSeq, true, 0);
        leftGenerator->setInfinite(false);
        leftGenerator->setHasEnd(true);
        leftGenerator->setEndIndex(leftSeq->getSize());
    } else {
        Sequence<T> *emptySeq = new MutableArraySequence<T>();
        leftGenerator = new BidirectionalGenerator<T>(this, emptySeq, true, 0);
        leftGenerator->setInfinite(false);
        leftGenerator->setHasEnd(true);
        leftGenerator->setEndIndex(0);
        delete emptySeq;
    }
    
    if (rightSeq != nullptr && rightSeq->getSize() > 0) {
        rightGenerator = new BidirectionalGenerator<T>(this, rightSeq, false, 0);
        rightGenerator->setInfinite(false);
        rightGenerator->setHasEnd(true);
        rightGenerator->setEndIndex(rightSeq->getSize());
    } else {
        Sequence<T> *emptySeq = new MutableArraySequence<T>();
        rightGenerator = new BidirectionalGenerator<T>(this, emptySeq, false, 0);
        rightGenerator->setInfinite(false);
        rightGenerator->setHasEnd(true);
        rightGenerator->setEndIndex(0);
        delete emptySeq;
    }
}

template<class T>
void TuringTape<T>::createRecurrentGenerators(std::function<T(Sequence<T>*)> leftRule,
                                               std::function<T(Sequence<T>*)> rightRule,
                                               Sequence<T> *initialLeftElements,
                                               Sequence<T> *initialRightElements) {
    // Создаем генераторы для рекуррентных правил
    if (leftRule != nullptr) {
        leftGenerator = new BidirectionalGenerator<T>(this, leftRule, initialLeftElements, true);
        // Материализуем начальные элементы
        if (initialLeftElements != nullptr) {
            for (int i = 0; i < initialLeftElements->getSize(); i++) {
                leftMaterialized->inPrepend(initialLeftElements->get(i));
            }
        }
    } else {
        Sequence<T> *emptySeq = new MutableArraySequence<T>();
        leftGenerator = new BidirectionalGenerator<T>(this, emptySeq, true, 0);
        leftGenerator->setInfinite(false);
        leftGenerator->setHasEnd(true);
        leftGenerator->setEndIndex(0);
        delete emptySeq;
        isLeftInfinite = false;
        leftLength = Cardinal(0);
    }
    
    if (rightRule != nullptr) {
        rightGenerator = new BidirectionalGenerator<T>(this, rightRule, initialRightElements, false);
        // Материализуем начальные элементы
        if (initialRightElements != nullptr) {
            for (int i = 0; i < initialRightElements->getSize(); i++) {
                rightMaterialized->inAppend(initialRightElements->get(i));
            }
        }
    } else {
        Sequence<T> *emptySeq = new MutableArraySequence<T>();
        rightGenerator = new BidirectionalGenerator<T>(this, emptySeq, false, 0);
        rightGenerator->setInfinite(false);
        rightGenerator->setHasEnd(true);
        rightGenerator->setEndIndex(0);
        delete emptySeq;
        isRightInfinite = false;
        rightLength = Cardinal(0);
    }
}
