#ifndef GENERATOR_H
#define GENERATOR_H

#include "../Sequence.h"
#include <functional>

template<class T>
class LazySequence;

enum class ModificationType {
    APPEND,
    INSERT,
    REMOVE,
    APPEND_SEQUENCE,
    INSERT_SEQUENCE,
    REMOVE_SEQUENCE
};

template<class T>
struct Modification {
    ModificationType type;
    size_t index; // позиция для Insert/Remove, игнорируется для Append
    T item; // для операций с одним элементом
    Sequence<T> *sequence; // для операций с последовательностью (nullptr если не используется)
    bool hasSequence;

    Modification() : type(ModificationType::APPEND), index(0), hasSequence(false), sequence(nullptr) {
    }
};

template<class T>
class Generator {
    friend class LazySequence<T>;

public:
    Generator(LazySequence<T> *owner,
              std::function<T(Sequence<T> *)> rule,
              Sequence<T> *initialElements);

    Generator(LazySequence<T> *owner,
              size_t baseIndex,
              Sequence<T> *baseSequence,
              const Modification<T> &modification);

    Generator(const Generator<T> &other);

    ~Generator();

    T GetNext();

    [[nodiscard]] bool HasNext() const;

    Generator *Append(const T &item) const;

    Generator *Append(Sequence<T> *items) const;

    Generator *Insert(const T &item, size_t index) const;

    Generator *Insert(Sequence<T> *items, size_t index) const;

    Generator *Remove(size_t index) const;

    Generator *Remove(size_t startIndex, size_t endIndex) const;

    void setInfinite(bool infinite);

    void setHasEnd(bool hasEnd);

    void setEndIndex(size_t index);

    void setSimpleSequence(bool simple);

    void setBaseSequence(Sequence<T> *seq);

    Sequence<T> *getBaseSequence() const;

    void setOwner(LazySequence<T> *newOwner);

private:
    LazySequence<T> *owner; // ссылка на хозяина
    std::function<T(Sequence<T> *)> generationRule; // правило порождения
    Sequence<T> *initialElements; // начальные элементы
    Sequence<T> *previousElements; // очередь фиксированной длины для предшествующих элементов
    size_t maxHistorySize; // максимальный размер истории (для рекуррентных правил)
    size_t currentIndex; // текущий индекс в генерации
    bool isInfinite; // является ли последовательность бесконечной
    bool hasEnd; // есть ли конец у последовательности
    size_t endIndex; // индекс конца (если hasEnd == true)

    // Для операций модификации и простых последовательностей
    bool isModificationGenerator; // является ли генератор модификационным
    size_t baseIndex; // базовая позиция для модификации
    Sequence<T> *baseSequence; // базовая последовательность (для модификаций и простых последовательностей)
    Modification<T> modification; // операция модификации
    bool isSimpleSequence; // является ли генератор простой последовательностью (не рекуррентной)

    void initHistoryQueue();

    void updateHistoryQueue(const T &newElement);

    T computeNext();

    T computeNextWithModifications();
};

#include "Generator.tpp"

#endif //GENERATOR_H
