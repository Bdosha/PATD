#pragma once
#include "BidirectionalGenerator.h"
#include "TuringTape.h"
#include "../Array/ArraySequences.h"
#include <stdexcept>

template<class T>
BidirectionalGenerator<T>::BidirectionalGenerator(TuringTape<T> *owner,
                                                   std::function<T(Sequence<T>*)> rule,
                                                   Sequence<T> *initialElements,
                                                   bool isLeftSide)
    : owner(owner),
      rule(rule),
      initialElements(initialElements != nullptr
                          ? new MutableArraySequence<T>(initialElements)
                          : new MutableArraySequence<T>()),
      previousElements(new MutableArraySequence<T>()),
      maxHistorySize(initialElements != nullptr ? initialElements->getSize() : 0),
      currentIndex(0),
      isInfinite(true),
      hasEnd(false),
      endIndex(0),
      isSimpleSequence(false),
      baseSequence(nullptr),
      startIndex(0),
      isLeftSide(isLeftSide) {
    initHistoryQueue();
}

template<class T>
BidirectionalGenerator<T>::BidirectionalGenerator(TuringTape<T> *owner,
                                                   Sequence<T> *baseSequence,
                                                   bool isLeftSide,
                                                   int startIndex)
    : owner(owner),
      rule(nullptr),
      initialElements(nullptr),
      previousElements(new MutableArraySequence<T>()),
      maxHistorySize(0),
      currentIndex(0),
      isInfinite(false),
      hasEnd(true),
      endIndex(0),
      isSimpleSequence(true),
      baseSequence(baseSequence != nullptr ? new MutableArraySequence<T>(baseSequence) : nullptr),
      startIndex(startIndex),
      isLeftSide(isLeftSide) {
    if (baseSequence != nullptr) {
        if (isLeftSide) {
            // Для левой стороны: генерируем все элементы из baseSequence
            endIndex = baseSequence->getSize();
        } else {
            // Для правой стороны: генерируем элементы от startIndex вверх
            endIndex = baseSequence->getSize() - startIndex;
        }
    }
}

template<class T>
BidirectionalGenerator<T>::BidirectionalGenerator(const BidirectionalGenerator<T> &other)
    : owner(other.owner),
      rule(other.rule),
      initialElements(other.initialElements != nullptr
                          ? new MutableArraySequence<T>(other.initialElements)
                          : nullptr),
      previousElements(other.previousElements != nullptr
                           ? new MutableArraySequence<T>(other.previousElements)
                           : new MutableArraySequence<T>()),
      maxHistorySize(other.maxHistorySize),
      currentIndex(other.currentIndex),
      isInfinite(other.isInfinite),
      hasEnd(other.hasEnd),
      endIndex(other.endIndex),
      isSimpleSequence(other.isSimpleSequence),
      baseSequence(other.baseSequence != nullptr
                       ? new MutableArraySequence<T>(other.baseSequence)
                       : nullptr),
      startIndex(other.startIndex),
      isLeftSide(other.isLeftSide) {
}

template<class T>
BidirectionalGenerator<T>::~BidirectionalGenerator() {
    if (initialElements != nullptr) {
        delete initialElements;
    }
    if (previousElements != nullptr) {
        delete previousElements;
    }
    if (baseSequence != nullptr) {
        delete baseSequence;
    }
}

template<class T>
void BidirectionalGenerator<T>::initHistoryQueue() {
    if (initialElements == nullptr) {
        return;
    }

    // Копируем начальные элементы в очередь истории
    for (int i = 0; i < initialElements->getSize(); i++) {
        previousElements->inAppend(initialElements->get(i));
    }
}

template<class T>
void BidirectionalGenerator<T>::updateHistoryQueue(const T &newElement) {
    if (previousElements == nullptr) {
        previousElements = new MutableArraySequence<T>();
    }

    previousElements->inAppend(newElement);

    // Если размер превышает maxHistorySize, удаляем первый элемент
    if (maxHistorySize > 0 && previousElements->getSize() > maxHistorySize) {
        previousElements->inRemove(0);
    }
}

template<class T>
T BidirectionalGenerator<T>::computeNext() {
    if (isSimpleSequence && baseSequence != nullptr) {
        // Для простой последовательности
        if (isLeftSide) {
            // Для левой стороны: baseSequence хранит элементы от меньших к большим индексам
            // baseSequence[0] = элемент с наименьшим индексом
            // baseSequence[i] = элемент с индексом -baseSequence->getSize() + i
            // Генерируем элементы от меньших к большим индексам
            int targetIndex = static_cast<int>(currentIndex);
            if (targetIndex < 0 || targetIndex >= baseSequence->getSize()) {
                throw std::out_of_range("Index out of range in base sequence");
            }
            currentIndex++;
            return baseSequence->get(targetIndex);
        } else {
            // Для правой стороны: генерируем от startIndex вверх
            int targetIndex = startIndex + static_cast<int>(currentIndex);
            if (targetIndex < 0 || targetIndex >= baseSequence->getSize()) {
                throw std::out_of_range("Index out of range in base sequence");
            }
            currentIndex++;
            return baseSequence->get(targetIndex);
        }
    }

    // Для рекуррентного правила
    if (rule == nullptr) {
        throw std::runtime_error("No generation rule specified");
    }

    // Получаем историю из владельца
    Sequence<T> *history = nullptr;
    if (owner != nullptr) {
        if (isLeftSide) {
            // Для левой стороны получаем уже вычисленные элементы
            history = owner->getLeftMaterialized();
        } else {
            // Для правой стороны получаем уже вычисленные элементы
            history = owner->getRightMaterialized();
        }
    }

    if (history == nullptr || history->getSize() == 0) {
        // Если истории нет, используем начальные элементы
        history = previousElements;
    }

    // Вычисляем следующий элемент по правилу
    T next = rule(history);
    updateHistoryQueue(next);
    currentIndex++;

    return next;
}

template<class T>
T BidirectionalGenerator<T>::GetNext() {
    if (!HasNext()) {
        throw std::out_of_range("No next element available");
    }

    return computeNext();
}

template<class T>
bool BidirectionalGenerator<T>::HasNext() const {
    if (isInfinite) {
        return true;
    }

    if (!hasEnd) {
        return true;
    }

    if (isSimpleSequence && baseSequence != nullptr) {
        if (isLeftSide) {
            // Для левой стороны: проверяем, не достигли ли мы конца последовательности
            int targetIndex = static_cast<int>(currentIndex);
            return targetIndex >= 0 && targetIndex < baseSequence->getSize();
        } else {
            // Для правой стороны: проверяем, не достигли ли мы конца
            int targetIndex = startIndex + static_cast<int>(currentIndex);
            return targetIndex >= 0 && targetIndex < baseSequence->getSize();
        }
    }

    return currentIndex < endIndex;
}

template<class T>
void BidirectionalGenerator<T>::setOwner(TuringTape<T> *newOwner) {
    owner = newOwner;
}

template<class T>
void BidirectionalGenerator<T>::setInfinite(bool infinite) {
    isInfinite = infinite;
}

template<class T>
void BidirectionalGenerator<T>::setHasEnd(bool hasEnd) {
    this->hasEnd = hasEnd;
}

template<class T>
void BidirectionalGenerator<T>::setEndIndex(size_t index) {
    endIndex = index;
}

template<class T>
void BidirectionalGenerator<T>::setSimpleSequence(bool simple) {
    isSimpleSequence = simple;
}

template<class T>
Sequence<T> *BidirectionalGenerator<T>::getBaseSequence() const {
    return baseSequence;
}
