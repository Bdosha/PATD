#ifndef BIDIRECTIONALGENERATOR_H
#define BIDIRECTIONALGENERATOR_H

#include "../Sequence.h"
#include <functional>
#include <stdexcept>

// Forward declaration
template<class T>
class TuringTape;

template<class T>
class BidirectionalGenerator {
    friend class TuringTape<T>;

public:
    BidirectionalGenerator(TuringTape<T> *owner,
                           std::function<T(Sequence<T>*)> rule,
                           Sequence<T> *initialElements,
                           bool isLeftSide);

    BidirectionalGenerator(TuringTape<T> *owner,
                           Sequence<T> *baseSequence,
                           bool isLeftSide,
                           int startIndex = 0);

    BidirectionalGenerator(const BidirectionalGenerator<T> &other);

    ~BidirectionalGenerator();

    T GetNext();

    [[nodiscard]] bool HasNext() const;

    void setOwner(TuringTape<T> *newOwner);

    void setInfinite(bool infinite);

    void setHasEnd(bool hasEnd);

    void setEndIndex(size_t index);

    void setSimpleSequence(bool simple);

    Sequence<T> *getBaseSequence() const;

private:
    TuringTape<T> *owner;                    // ссылка на хозяина
    std::function<T(Sequence<T>*)> rule;     // правило порождения
    Sequence<T> *initialElements;            // начальные элементы
    Sequence<T> *previousElements;           // очередь для предшествующих элементов
    size_t maxHistorySize;                    // максимальный размер истории
    size_t currentIndex;                      // текущий индекс генерации
    bool isInfinite;                          // бесконечна ли последовательность
    bool hasEnd;                              // есть ли конец
    size_t endIndex;                          // индекс конца

    // Для простых последовательностей
    bool isSimpleSequence;                    // является ли простой последовательностью
    Sequence<T> *baseSequence;                // базовая последовательность
    int startIndex;                           // начальный индекс в базовой последовательности
    bool isLeftSide;                          // является ли генератором для левой стороны

    void initHistoryQueue();

    void updateHistoryQueue(const T &newElement);

    T computeNext();
};

#include "BidirectionalGenerator.tpp"

#endif //BIDIRECTIONALGENERATOR_H
