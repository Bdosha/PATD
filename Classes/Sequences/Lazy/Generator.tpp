#pragma once
#include "Generator.h"
#include "../Array/ArraySequences.h"
#include <stdexcept>

template<class T>
Generator<T>::Generator(LazySequence<T> *owner,
                        std::function<T(Sequence<T> *)> rule,
                        Sequence<T> *initialElements)
    : owner(owner),
      generationRule(rule),
      initialElements(initialElements != nullptr
                          ? new MutableArraySequence<T>(initialElements)
                          : new MutableArraySequence<T>()),
      previousElements(new MutableArraySequence<T>()),
      maxHistorySize(initialElements != nullptr ? initialElements->getSize() : 0),
      currentIndex(0),
      isInfinite(true),
      hasEnd(false),
      endIndex(0),
      isModificationGenerator(false),
      baseIndex(0),
      baseSequence(nullptr),
      isSimpleSequence(false) {
    initHistoryQueue();
}

template<class T>
Generator<T>::Generator(LazySequence<T> *owner,
                        size_t baseIndex,
                        Sequence<T> *baseSequence,
                        const Modification<T> &modification)
    : owner(owner),
      generationRule(nullptr),
      initialElements(nullptr),
      previousElements(new MutableArraySequence<T>()),
      maxHistorySize(0),
      currentIndex(0),
      isInfinite(false),
      hasEnd(true),
      endIndex(0),
      isModificationGenerator(true),
      baseIndex(baseIndex),
      baseSequence(baseSequence != nullptr ? new MutableArraySequence<T>(baseSequence) : nullptr),
      modification(modification),
      isSimpleSequence(false) {
    // Вычисляем конечный индекс с учетом модификации
    if (baseSequence != nullptr) {
        size_t baseSize = baseSequence->getSize();
        switch (modification.type) {
            case ModificationType::APPEND:
            case ModificationType::APPEND_SEQUENCE:
                endIndex = baseSize + 1; // будет обновлено при вычислении
                break;
            case ModificationType::INSERT:
            case ModificationType::INSERT_SEQUENCE:
                endIndex = baseSize + 1; // будет обновлено при вычислении
                break;
            case ModificationType::REMOVE:
                endIndex = baseSize - 1;
                break;
            case ModificationType::REMOVE_SEQUENCE:
                endIndex = baseSize - 1; // приблизительно
                break;
            default: ;
        }
    }
}

template<class T>
Generator<T>::Generator(const Generator<T> &other)
    : owner(other.owner),
      generationRule(other.generationRule),
      initialElements(other.initialElements != nullptr ? new MutableArraySequence<T>(other.initialElements) : nullptr),
      previousElements(other.previousElements != nullptr
                           ? new MutableArraySequence<T>(other.previousElements)
                           : new MutableArraySequence<T>()),
      maxHistorySize(other.maxHistorySize),
      currentIndex(other.currentIndex),
      isInfinite(other.isInfinite),
      hasEnd(other.hasEnd),
      endIndex(other.endIndex),
      isModificationGenerator(other.isModificationGenerator),
      baseIndex(other.baseIndex),
      baseSequence(other.baseSequence != nullptr ? new MutableArraySequence<T>(other.baseSequence) : nullptr),
      modification(other.modification),
      isSimpleSequence(other.isSimpleSequence) {
    // Копируем sequence из modification, если он есть
    if (modification.hasSequence && modification.sequence != nullptr) {
        modification.sequence = new MutableArraySequence<T>(modification.sequence);
    } else {
        modification.sequence = nullptr;
    }
}

template<class T>
Generator<T>::~Generator() {
    if (initialElements != nullptr) {
        delete initialElements;
    }
    if (previousElements != nullptr) {
        delete previousElements;
    }
    if (baseSequence != nullptr) {
        delete baseSequence;
    }
    if (modification.hasSequence && modification.sequence != nullptr) {
        delete modification.sequence;
    }
}

template<class T>
void Generator<T>::initHistoryQueue() {
    if (initialElements != nullptr && initialElements->getSize() > 0) {
        // Копируем начальные элементы в очередь истории
        for (int i = 0; i < initialElements->getSize(); i++) {
            previousElements->inAppend(initialElements->get(i));
        }
        maxHistorySize = initialElements->getSize();
    }
}

template<class T>
void Generator<T>::updateHistoryQueue(const T &newElement) {
    if (maxHistorySize == 0) return;

    // Добавляем новый элемент
    previousElements->inAppend(newElement);

    // Если очередь превысила максимальный размер, удаляем первый элемент
    if (previousElements->getSize() > maxHistorySize) {
        previousElements->inRemove(0);
    }
}

template<class T>
T Generator<T>::computeNext() {
    if (generationRule == nullptr) {
        throw std::runtime_error("Generation rule is not set");
    }

    // Используем очередь предшествующих элементов для вычисления следующего
    T next = generationRule(previousElements);
    updateHistoryQueue(next);
    currentIndex++;
    return next;
}

template<class T>
T Generator<T>::computeNextWithModifications() {
    if (baseSequence == nullptr) {
        throw std::out_of_range("End of sequence reached");
    }

    size_t effectiveIndex = currentIndex;
    size_t baseSize = baseSequence->getSize();

    // Применяем модификацию
    switch (modification.type) {
        case ModificationType::APPEND: {
            if (effectiveIndex < baseSize) {
                T result = baseSequence->get(effectiveIndex);
                currentIndex++;
                return result;
            } else if (effectiveIndex == baseSize) {
                currentIndex++;
                return modification.item;
            } else {
                throw std::out_of_range("End of sequence reached");
            }
        }

        case ModificationType::INSERT: {
            if (modification.index > baseSize) {
                throw std::out_of_range("Insert index out of range");
            }
            if (effectiveIndex < modification.index) {
                T result = baseSequence->get(effectiveIndex);
                currentIndex++;
                return result;
            } else if (effectiveIndex == modification.index) {
                currentIndex++;
                return modification.item;
            } else if (effectiveIndex < baseSize + 1) {
                T result = baseSequence->get(effectiveIndex - 1);
                currentIndex++;
                return result;
            } else {
                throw std::out_of_range("End of sequence reached");
            }
        }

        case ModificationType::REMOVE: {
            if (modification.index >= baseSize) {
                throw std::out_of_range("Remove index out of range");
            }
            if (effectiveIndex < modification.index) {
                T result = baseSequence->get(effectiveIndex);
                currentIndex++;
                return result;
            } else if (effectiveIndex < baseSize - 1) {
                T result = baseSequence->get(effectiveIndex + 1);
                currentIndex++;
                return result;
            } else {
                throw std::out_of_range("End of sequence reached");
            }
        }

        case ModificationType::APPEND_SEQUENCE: {
            if (modification.sequence == nullptr) {
                throw std::runtime_error("Sequence is null");
            }
            if (effectiveIndex < baseSize) {
                T result = baseSequence->get(effectiveIndex);
                currentIndex++;
                return result;
            } else if (effectiveIndex < baseSize + modification.sequence->getSize()) {
                T result = modification.sequence->get(effectiveIndex - baseSize);
                currentIndex++;
                return result;
            } else {
                throw std::out_of_range("End of sequence reached");
            }
        }

        case ModificationType::INSERT_SEQUENCE: {
            if (modification.sequence == nullptr) {
                throw std::runtime_error("Sequence is null");
            }
            if (modification.index > baseSize) {
                throw std::out_of_range("Insert index out of range");
            }
            if (effectiveIndex < modification.index) {
                T result = baseSequence->get(effectiveIndex);
                currentIndex++;
                return result;
            } else if (effectiveIndex < modification.index + modification.sequence->getSize()) {
                T result = modification.sequence->get(effectiveIndex - modification.index);
                currentIndex++;
                return result;
            } else if (effectiveIndex < baseSize + modification.sequence->getSize()) {
                T result = baseSequence->get(effectiveIndex - modification.sequence->getSize());
                currentIndex++;
                return result;
            } else {
                throw std::out_of_range("End of sequence reached");
            }
        }

        case ModificationType::REMOVE_SEQUENCE: {
            // Упрощенная реализация - удаляем один элемент
            if (modification.index >= baseSize) {
                throw std::out_of_range("Remove index out of range");
            }
            if (effectiveIndex < modification.index) {
                T result = baseSequence->get(effectiveIndex);
                currentIndex++;
                return result;
            } else if (effectiveIndex < baseSize - 1) {
                T result = baseSequence->get(effectiveIndex + 1);
                currentIndex++;
                return result;
            } else {
                throw std::out_of_range("End of sequence reached");
            }
        }

        default:
            throw std::runtime_error("Unknown modification type");
    }
}

template<class T>
T Generator<T>::GetNext() {
    if (isSimpleSequence && baseSequence != nullptr) {
        // Для простой последовательности просто возвращаем элементы по индексу
        if (currentIndex >= baseSequence->getSize()) {
            throw std::out_of_range("End of sequence reached");
        }
        T result = baseSequence->get(currentIndex);
        currentIndex++;
        return result;
    } else if (isModificationGenerator) {
        if (!HasNext()) {
            throw std::out_of_range("End of sequence reached");
        }
        return computeNextWithModifications();
    } else {
        if (!HasNext()) {
            throw std::out_of_range("End of sequence reached");
        }
        return computeNext();
    }
}

template<class T>
bool Generator<T>::HasNext() const {
    if (isSimpleSequence && baseSequence != nullptr) {
        return currentIndex < baseSequence->getSize();
    } else if (isModificationGenerator) {
        return currentIndex < endIndex;
    } else {
        // Для рекуррентных правил считаем бесконечными, если не указано иное
        return isInfinite || !hasEnd || currentIndex < endIndex;
    }
}

template<class T>
Generator<T> *Generator<T>::Append(const T &item) const {
    Modification<T> mod;
    mod.type = ModificationType::APPEND;
    mod.item = item;
    mod.hasSequence = false;
    mod.sequence = nullptr;

    // Вычисляем базовую позицию
    size_t newBaseIndex = isModificationGenerator ? baseIndex + currentIndex : currentIndex;
    Sequence<T> *newBaseSequence = nullptr;
    if (isModificationGenerator) {
        newBaseSequence = baseSequence != nullptr
                              ? new MutableArraySequence<T>(baseSequence)
                              : new MutableArraySequence<T>();
    } else {
        newBaseSequence = initialElements != nullptr
                              ? new MutableArraySequence<T>(initialElements)
                              : new MutableArraySequence<T>();
    }

    return new Generator<T>(owner, newBaseIndex, newBaseSequence, mod);
}

template<class T>
Generator<T> *Generator<T>::Append(Sequence<T> *items) const {
    if (items == nullptr) {
        throw std::invalid_argument("Items sequence is null");
    }

    Modification<T> mod;
    mod.type = ModificationType::APPEND_SEQUENCE;
    mod.hasSequence = true;
    mod.sequence = new MutableArraySequence<T>(items);

    size_t newBaseIndex = isModificationGenerator ? baseIndex + currentIndex : currentIndex;
    Sequence<T> *newBaseSequence = nullptr;
    if (isModificationGenerator) {
        newBaseSequence = baseSequence != nullptr
                              ? new MutableArraySequence<T>(baseSequence)
                              : new MutableArraySequence<T>();
    } else {
        newBaseSequence = initialElements != nullptr
                              ? new MutableArraySequence<T>(initialElements)
                              : new MutableArraySequence<T>();
    }

    return new Generator<T>(owner, newBaseIndex, newBaseSequence, mod);
}

template<class T>
Generator<T> *Generator<T>::Insert(const T &item, size_t index) const {
    Modification<T> mod;
    mod.type = ModificationType::INSERT;
    mod.index = index;
    mod.item = item;
    mod.hasSequence = false;
    mod.sequence = nullptr;

    size_t newBaseIndex = isModificationGenerator ? baseIndex + currentIndex : currentIndex;
    Sequence<T> *newBaseSequence = nullptr;
    if (isModificationGenerator) {
        newBaseSequence = baseSequence != nullptr
                              ? new MutableArraySequence<T>(baseSequence)
                              : new MutableArraySequence<T>();
    } else {
        newBaseSequence = initialElements != nullptr
                              ? new MutableArraySequence<T>(initialElements)
                              : new MutableArraySequence<T>();
    }

    return new Generator<T>(owner, newBaseIndex, newBaseSequence, mod);
}

template<class T>
Generator<T> *Generator<T>::Insert(Sequence<T> *items, size_t index) const {
    if (items == nullptr) {
        throw std::invalid_argument("Items sequence is null");
    }

    Modification<T> mod;
    mod.type = ModificationType::INSERT_SEQUENCE;
    mod.index = index;
    mod.hasSequence = true;
    mod.sequence = new MutableArraySequence<T>(items);

    size_t newBaseIndex = isModificationGenerator ? baseIndex + currentIndex : currentIndex;
    Sequence<T> *newBaseSequence = nullptr;
    if (isModificationGenerator) {
        newBaseSequence = baseSequence != nullptr
                              ? new MutableArraySequence<T>(baseSequence)
                              : new MutableArraySequence<T>();
    } else {
        newBaseSequence = initialElements != nullptr
                              ? new MutableArraySequence<T>(initialElements)
                              : new MutableArraySequence<T>();
    }

    return new Generator<T>(owner, newBaseIndex, newBaseSequence, mod);
}

template<class T>
Generator<T> *Generator<T>::Remove(size_t index) const {
    Modification<T> mod;
    mod.type = ModificationType::REMOVE;
    mod.index = index;
    mod.hasSequence = false;
    mod.sequence = nullptr;

    size_t newBaseIndex = isModificationGenerator ? baseIndex + currentIndex : currentIndex;
    Sequence<T> *newBaseSequence = nullptr;
    if (isModificationGenerator) {
        newBaseSequence = baseSequence != nullptr
                              ? new MutableArraySequence<T>(baseSequence)
                              : new MutableArraySequence<T>();
    } else {
        newBaseSequence = initialElements != nullptr
                              ? new MutableArraySequence<T>(initialElements)
                              : new MutableArraySequence<T>();
    }

    return new Generator<T>(owner, newBaseIndex, newBaseSequence, mod);
}

template<class T>
Generator<T> *Generator<T>::Remove(size_t startIndex, size_t endIndex) const {
    // Упрощенная реализация - удаляем один элемент
    return Remove(startIndex);
}

template<class T>
void Generator<T>::setInfinite(bool infinite) {
    isInfinite = infinite;
}

template<class T>
void Generator<T>::setHasEnd(bool hasEnd) {
    this->hasEnd = hasEnd;
}

template<class T>
void Generator<T>::setEndIndex(size_t index) {
    endIndex = index;
}

template<class T>
void Generator<T>::setSimpleSequence(bool simple) {
    isSimpleSequence = simple;
}

template<class T>
void Generator<T>::setBaseSequence(Sequence<T> *seq) {
    if (baseSequence != nullptr && baseSequence != initialElements) {
        delete baseSequence;
    }
    baseSequence = seq != nullptr ? new MutableArraySequence<T>(seq) : nullptr;
}

template<class T>
Sequence<T> *Generator<T>::getBaseSequence() const {
    return baseSequence;
}

template<class T>
void Generator<T>::setOwner(LazySequence<T> *newOwner) {
    owner = newOwner;
}
