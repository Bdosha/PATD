#ifndef TURINGTAPE_H
#define TURINGTAPE_H

#include "../Sequence.h"
#include "Generator.h"
#include "Cardinal.h"
#include <functional>
#include <stdexcept>

// Forward declaration
template<class T>
class BidirectionalGenerator;

template<class T>
class TuringTape {
    friend class BidirectionalGenerator<T>;

public:
    explicit TuringTape(T initialValue = T());

    TuringTape(T *items, int count, int startIndex = 0);

    explicit TuringTape(Sequence<T> *seq, int startIndex = 0);

    TuringTape(std::function<T(Sequence<T>*)> leftRule,
               std::function<T(Sequence<T>*)> rightRule,
               Sequence<T> *initialLeftElements,
               Sequence<T> *initialRightElements);

    TuringTape(const TuringTape<T> &other);

    ~TuringTape();

    T Get(int index);

    void Set(int index, T value);

    TuringTape<T> *InsertAt(int index, T item);

    TuringTape<T> *Remove(int index);

    TuringTape<T> *GetSubsequence(int startIndex, int endIndex);

    [[nodiscard]] Cardinal GetLeftLength() const;

    [[nodiscard]] Cardinal GetRightLength() const;

    [[nodiscard]] Cardinal GetTotalLength() const;

    [[nodiscard]] size_t GetLeftMaterializedCount() const;

    [[nodiscard]] size_t GetRightMaterializedCount() const;

    [[nodiscard]] size_t GetTotalMaterializedCount() const;

    [[nodiscard]] bool IsLeftInfinite() const;

    [[nodiscard]] bool IsRightInfinite() const;

    Sequence<T> *getLeftMaterialized() const { return leftMaterialized; }
    Sequence<T> *getRightMaterialized() const { return rightMaterialized; }

    T operator[](int index);

    template<class T2>
    TuringTape<T2> *Map(T2 (*func)(T));

    TuringTape<T> *Where(bool (*predicate)(T));

private:
    BidirectionalGenerator<T> *leftGenerator;   // для отрицательных индексов
    BidirectionalGenerator<T> *rightGenerator;   // для неотрицательных индексов

    Sequence<T> *leftMaterialized;   // элементы с отрицательными индексами (от меньших к большим)
                                      // leftMaterialized[0] = элемент с индексом -leftMaterialized->getSize()
                                      // leftMaterialized[i] = элемент с индексом -leftMaterialized->getSize() + i
    Sequence<T> *rightMaterialized;   // элементы с неотрицательными индексами
                                      // rightMaterialized[0] = элемент с индексом 0
                                      // rightMaterialized[i] = элемент с индексом i

    // Флаги бесконечности
    bool isLeftInfinite;   // бесконечна ли левая сторона
    bool isRightInfinite;  // бесконечна ли правая сторона

    // Длины
    Cardinal leftLength;   // длина левой стороны
    Cardinal rightLength;  // длина правой стороны

    void materializeLeftUpTo(int index);

    void materializeRightUpTo(int index);

    [[nodiscard]] int leftIndexToMaterialized(int index) const;

    [[nodiscard]] int rightIndexToMaterialized(int index) const;

    void createSimpleGenerators(Sequence<T> *leftSeq, Sequence<T> *rightSeq, int startIndex);

    void createRecurrentGenerators(std::function<T(Sequence<T>*)> leftRule,
                                   std::function<T(Sequence<T>*)> rightRule,
                                   Sequence<T> *initialLeftElements,
                                   Sequence<T> *initialRightElements);
};

#include "TuringTape.tpp"

#endif //TURINGTAPE_H
